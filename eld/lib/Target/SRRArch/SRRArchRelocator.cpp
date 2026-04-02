//===-SRRArchRelocator.cpp-------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "SRRArchRelocator.h"
#include "SRRArchLDBackend.h"
#include "SRRArchLLVMExtern.h"
#include "SRRArchPLT.h"
#include "SRRArchRelocationFunctions.h"
#include "SRRArchRelocationInfo.h"
#include "eld/Diagnostics/DiagnosticEngine.h"
#include "eld/Input/ELFObjectFile.h"
#include "eld/Support/MsgHandling.h"
#include "eld/SymbolResolver/LDSymbol.h"
#include "eld/SymbolResolver/Resolver.h"
#include "llvm/ADT/Twine.h"
#include "llvm/BinaryFormat/ELF.h"

using namespace eld;

namespace {} // anonymous namespace

//===--------------------------------------------------------------------===//
// SRRArchRelocator
//===--------------------------------------------------------------------===//
SRRArchRelocator::SRRArchRelocator(SRRArchLDBackend &Backend,
                                   LinkerConfig &pConfig, Module &pModule)
    : Relocator(pConfig, pModule), m_Target(Backend) {
  // Mark force verify bit for specified relocations
  if (m_Module.getPrinter()->verifyReloc() &&
      config().options().verifyRelocList().size()) {
    auto &list = config().options().verifyRelocList();
    for (auto &i : SRRARCHRelocDesc) {
      auto RelocInfo = SRRArchRelocs[i.type];
      if (list.find(RelocInfo.Name) != list.end())
        i.forceVerify = true;
    }
  }
}

SRRArchRelocator::~SRRArchRelocator() {}

namespace {} // namespace

Relocator::Result SRRArchRelocator::applyRelocation(Relocation &pRelocation) {
  Relocation::Type type = pRelocation.type();

  ResolveInfo *symInfo = pRelocation.symInfo();

  if (type > SRRARCH_MAXRELOCS)
    return Relocator::Unknown;

  if (symInfo) {
    LDSymbol *outSymbol = symInfo->outSymbol();
    if (outSymbol && outSymbol->hasFragRef()) {
      ELFSection *S = outSymbol->fragRef()->frag()->getOwningSection();
      if (S->isDiscard() ||
          (S->getOutputSection() && S->getOutputSection()->isDiscard())) {
        std::lock_guard<std::mutex> relocGuard(m_RelocMutex);
        issueUndefRef(pRelocation, *S->getInputFile(), S);
        return Relocator::OK;
      }
    }
  }

  // apply the relocation
  return SRRARCHRelocDesc[type].func(pRelocation, *this,
                                     SRRARCHRelocDesc[type]);
}

const char *SRRArchRelocator::getName(Relocation::Type pType) const {
  llvm_unreachable("getName not implemented yet.");
}

SRRArchLDBackend &SRRArchRelocator::getTarget() { return m_Target; }

const SRRArchLDBackend &SRRArchRelocator::getTarget() const { return m_Target; }

bool SRRArchRelocator::isRelocSupported(Relocation &pReloc) const {
  return pReloc.type() < SRRARCH_MAXRELOCS;
}

void SRRArchRelocator::scanRelocation(Relocation &pReloc,
                                      eld::IRBuilder &pLinker,
                                      ELFSection &pSection,
                                      InputFile &pInputFile,
                                      CopyRelocs &CopyRelocs) {
  if (LinkerConfig::Object == config().codeGenType())
    return;

  if (!isRelocSupported(pReloc)) {
    config().raise(Diag::unsupported_reloc)
        << pReloc.type() << pSection.getDecoratedName(config().options())
        << pInputFile.getInput()->decoratedPath();
    return;
  }

  // TODO: Implement
  // If we are generating a shared library check for invalid relocations
  // if (isInvalidReloc(pReloc)) {
  //   std::lock_guard<std::mutex> relocGuard(m_RelocMutex);
  //   config().raise(Diag::non_pic_relocation)
  //       << getName(pReloc.type()) << pReloc.symInfo()->name()
  //       << pReloc.getSourcePath(config().options());
  //   m_Target.getModule().setFailure(true);
  //   return;
  // }

  // rsym - The relocation target symbol
  ResolveInfo *rsym = pReloc.symInfo();
  assert(nullptr != rsym &&
         "ResolveInfo of relocation not set while scanRelocation");

  // Check if we are tracing relocations.
  if (m_Module.getPrinter()->traceReloc()) {
    std::lock_guard<std::mutex> relocGuard(m_RelocMutex);
    std::string relocName = getName(pReloc.type());
    if (config().options().traceReloc(relocName))
      config().raise(Diag::reloc_trace)
          << relocName << pReloc.symInfo()->name()
          << pInputFile.getInput()->decoratedPath();
  }

  // check if we should issue undefined reference for the relocation target
  // symbol
  {
    if (rsym->isUndef() || rsym->isBitCode()) {
      std::lock_guard<std::mutex> relocGuard(m_RelocMutex);
      if (m_Target.canIssueUndef(rsym)) {
        if (rsym->visibility() != ResolveInfo::Default)
          issueInvisibleRef(pReloc, pInputFile);
        issueUndefRef(pReloc, pInputFile, &pSection);
      }
    }
  }
  ELFSection *section = pSection.getLink()
                            ? pSection.getLink()
                            : pReloc.targetRef()->frag()->getOwningSection();

  if (!section->isAlloc())
    return;

  // TODO: Implement
  // if (rsym->isLocal()) // rsym is local
  //   scanLocalReloc(pInputFile, pReloc, pLinker, *section);
  // else // rsym is external
  //   scanGlobalReloc(pInputFile, pReloc, pLinker, *section, CopyRelocs);
}

uint32_t SRRArchRelocator::getNumRelocs() const {
  llvm_unreachable("getNumRelocs not implemented yet.");
}

Relocation::Size SRRArchRelocator::getSize(Relocation::Type pType) const {
  return SRRArchRelocs[pType].Size;
}

void SRRArchRelocator::partialScanRelocation(Relocation &pReloc,
                                             const ELFSection &pSection) {
  llvm_unreachable("partialScanRelocation not implemented yet.");
}

//=========================================//
// Relocation Verifier
//=========================================//

Relocator::Result ApplyReloc(Relocation &pReloc, uint32_t Result,
                             const RelocationDescription &pRelocDesc,
                             DiagnosticEngine *DiagEngine,
                             const GeneralOptions &options,
                             SRRArchRelocator &Parent) {
  auto RelocInfo = SRRArchRelocs[pReloc.type()];

  // Verify the Relocation.
  Relocator::Result R = Relocator::OK;
  // if (RelocInfo.IsSigned)
  //   R = VerifyRelocAsNeededHelper<int32_t>(pReloc, Result, pRelocDesc,
  //                                          DiagEngine, options, Parent);
  // else
  //   R = VerifyRelocAsNeededHelper<uint32_t>(pReloc, Result, pRelocDesc,
  //                                           DiagEngine, options, Parent);
  // if (R != Relocator::OK)
  //   return R;

  // Apply the relocation.
  pReloc.target() = doRelocSRRArch(RelocInfo, pReloc.target(), Result);
  return R;
}

//=========================================//
// Each relocation function implementation //
//=========================================//
Relocator::Result eld::none(Relocation &pReloc, SRRArchRelocator &pParent,
                            RelocationDescription &pRelocDesc) {
  return Relocator::OK;
}

Relocator::Result eld::relocAbs(Relocation &pReloc, SRRArchRelocator &pParent,
                                RelocationDescription &pRelocDesc) {
  DiagnosticEngine *DiagEngine = pParent.config().getDiagEngine();
  ResolveInfo *rsym = pReloc.symInfo();
  Relocator::Address S = pParent.getSymValue(&pReloc);
  Relocator::DWord A = pReloc.addend();

  const GeneralOptions &options = pParent.config().options();
  // For absolute relocations, and If we are building a static executable and if
  // the symbol is a weak undefined symbol, it should still use the undefined
  // symbol value which is 0. For non absolute relocations, the call is set to a
  // symbol defined by the linker which returns back to the caller.
  if (rsym && rsym->isWeakUndef() &&
      (pParent.config().codeGenType() == LinkerConfig::Exec)) {
    S = 0;
    return ApplyReloc(pReloc, S + A, pRelocDesc, DiagEngine, options, pParent);
  }

  // if the flag of target section is not ALLOC, we perform only static
  // relocation.
  if (!pReloc.targetRef()->getOutputELFSection()->isAlloc()) {
    return ApplyReloc(pReloc, S + A, pRelocDesc, DiagEngine, options, pParent);
  }

  // FIXME PLT STUFF
  // if (rsym && rsym->reserved() & Relocator::ReservePLT)
  //   S = pParent.getTarget().findEntryInPLT(rsym)->getAddr(DiagEngine);

  return ApplyReloc(pReloc, S + A, pRelocDesc, DiagEngine, options, pParent);
}

Relocator::Result eld::unsupport(Relocation &pReloc, SRRArchRelocator &pParent,
                                 RelocationDescription &pRelocDesc) {
  return SRRArchRelocator::Unsupport;
}
