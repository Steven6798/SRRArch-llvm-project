//===-SRRArchLDBackend.cpp-------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "SRRArchLDBackend.h"
#include "SRRArch.h"
#include "SRRArchAttributeFragment.h"
#include "SRRArchELFDynamic.h"
#include "SRRArchGOT.h"
#include "SRRArchLLVMExtern.h"
#include "SRRArchPLT.h"
#include "SRRArchRelocationHelper.h"
#include "SRRArchRelocationInternal.h"
#include "SRRArchRelocator.h"
#include "SRRArchStandaloneInfo.h"
#include "eld/Config/LinkerConfig.h"
#include "eld/Fragment/FillFragment.h"
#include "eld/Fragment/RegionFragment.h"
#include "eld/Fragment/RegionFragmentEx.h"
#include "eld/Fragment/Stub.h"
#include "eld/Input/ELFObjectFile.h"
#include "eld/Object/ObjectBuilder.h"
#include "eld/Object/ObjectLinker.h"
#include "eld/Support/Memory.h"
#include "eld/Support/MemoryArea.h"
#include "eld/Support/MsgHandling.h"
#include "eld/Support/TargetRegistry.h"
#include "eld/Support/Utils.h"
#include "eld/SymbolResolver/IRBuilder.h"
#include "eld/Target/ELFFileFormat.h"
#include "eld/Target/ELFSegmentFactory.h"
#include "eld/Target/GNULDBackend.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Support/Casting.h"
#include <optional>
#include <string>

using namespace eld;
using namespace llvm;

//===----------------------------------------------------------------------===//
// SRRArchLDBackend
//===----------------------------------------------------------------------===//
SRRArchLDBackend::SRRArchLDBackend(eld::Module &pModule, SRRArchInfo *pInfo)
    : GNULDBackend(pModule, pInfo) {}

SRRArchLDBackend::~SRRArchLDBackend() {}

bool SRRArchLDBackend::initRelocator() {
  if (nullptr == m_pRelocator)
    m_pRelocator = make<SRRArchRelocator>(*this, config(), m_Module);

  return true;
}

Relocator *SRRArchLDBackend::getRelocator() const {
  assert(nullptr != m_pRelocator);
  return m_pRelocator;
}

Relocation::Type SRRArchLDBackend::getCopyRelType() const {
  // FIXME create a COPY relocation for SRRArch
  return llvm::ELF::R_SRRARCH_NONE;
}

void SRRArchLDBackend::initDynamicSections(ELFObjectFile &InputFile) {
  InputFile.setDynamicSections(
      *m_Module.createInternalSection(
          InputFile, LDFileFormat::Internal, ".got", llvm::ELF::SHT_PROGBITS,
          llvm::ELF::SHF_ALLOC | llvm::ELF::SHF_WRITE, 8),
      *m_Module.createInternalSection(
          InputFile, LDFileFormat::Internal, ".got.plt",
          llvm::ELF::SHT_PROGBITS, llvm::ELF::SHF_ALLOC | llvm::ELF::SHF_WRITE,
          8),
      *m_Module.createInternalSection(
          InputFile, LDFileFormat::Internal, ".plt", llvm::ELF::SHT_PROGBITS,
          llvm::ELF::SHF_ALLOC | llvm::ELF::SHF_EXECINSTR, 8),
      *m_Module.createInternalSection(
          InputFile, LDFileFormat::DynamicRelocation, ".rela.dyn",
          llvm::ELF::SHT_RELA, llvm::ELF::SHF_ALLOC, 8),
      *m_Module.createInternalSection(
          InputFile, LDFileFormat::DynamicRelocation, ".rela.plt",
          llvm::ELF::SHT_RELA, llvm::ELF::SHF_ALLOC, 8));
}

void SRRArchLDBackend::initTargetSections(ObjectBuilder &pBuilder) {}

void SRRArchLDBackend::initPatchSections(ELFObjectFile &InputFile) {
  llvm_unreachable("initPatchSections not implemented yet.");
}

void SRRArchLDBackend::initTargetSymbols() {
  if (config().codeGenType() == LinkerConfig::Object)
    return;

  m_pEndOfImage =
      m_Module.getIRBuilder()->addSymbol<IRBuilder::Force, IRBuilder::Resolve>(
          m_Module.getInternalInput(Module::Script), "___end",
          ResolveInfo::NoType, ResolveInfo::Define, ResolveInfo::Absolute,
          0x0, // size
          0x0, // value
          FragmentRef::null());
  if (m_pEndOfImage)
    m_pEndOfImage->setShouldIgnore(false);
}

bool SRRArchLDBackend::initBRIslandFactory() { return true; }

bool SRRArchLDBackend::initStubFactory() { return true; }

bool SRRArchLDBackend::readSection(InputFile &pInput, ELFSection *S) {
  eld::LayoutInfo *layoutInfo = m_Module.getLayoutInfo();
  if (S->isCode()) {
    const char *Buf = pInput.getCopyForWrite(S->offset(), S->size());
    eld::RegionFragmentEx *F =
        make<RegionFragmentEx>(Buf, S->size(), S, S->getAddrAlign());
    S->addFragment(F);
    if (layoutInfo)
      layoutInfo->recordFragment(&pInput, S, F);
    return true;
  }
  return GNULDBackend::readSection(pInput, S);
}

ELFSection *SRRArchLDBackend::mergeSection(ELFSection *S) {
  llvm_unreachable("mergeSection not implemented yet.");
}

void SRRArchLDBackend::mayBeRelax(int relaxation_pass, bool &pFinished) {
  // TODO: Implement.
  pFinished = true;
}

/// finalizeSymbol - finalize the symbol value
bool SRRArchLDBackend::finalizeTargetSymbols() {
  if (config().codeGenType() == LinkerConfig::Object)
    return true;

  // Get the pointer to the real end of the image.
  if (m_pEndOfImage && !m_pEndOfImage->scriptDefined()) {
    uint64_t imageEnd = 0;
    for (auto &seg : elfSegmentTable()) {
      if (seg->type() != llvm::ELF::PT_LOAD)
        continue;
      uint64_t segSz = seg->paddr() + seg->memsz();
      if (imageEnd < segSz)
        imageEnd = segSz;
    }
    alignAddress(imageEnd, 8);
    m_pEndOfImage->setValue(imageEnd + 1);
  }

  if (m_Module.getScript().linkerScriptHasSectionsCommand())
    return true;

  return true;
}

void SRRArchLDBackend::initializeAttributes() {
  getInfo().initializeAttributes(m_Module.getIRBuilder()->getInputBuilder());
}

bool SRRArchLDBackend::validateArchOpts() const {
  llvm_unreachable("validateArchOpts not implemented yet.");
}

bool SRRArchLDBackend::handleRelocation(ELFSection *pSection,
                                        Relocation::Type pType, LDSymbol &pSym,
                                        uint32_t pOffset,
                                        Relocation::Address pAddend,
                                        bool pLastVisit) {
  return false;
}

Relocation::Type SRRArchLDBackend::getRemappedInternalRelocationType(
    Relocation::Type pType) const {
  llvm_unreachable("getRemappedInternalRelocationType not implemented yet.");
}

void SRRArchLDBackend::doPreLayout() {
  if (config().isCodeStatic() && !config().options().forceDynamic())
    return;
  llvm_unreachable("doPreLayout not implemented yet.");
}

bool SRRArchLDBackend::finalizeScanRelocations() {
  // FIXME Implement GOT and PLT stuff
  // Fragment *frag = nullptr;
  // if (auto *GOTPLT = getGOTPLT())
  //   if (GOTPLT->hasSectionData())
  //     frag = *GOTPLT->getFragmentList().begin();
  // if (frag)
  //   defineGOTSymbol(*frag);
  return true;
}

uint64_t
SRRArchLDBackend::getValueForDiscardedRelocations(const Relocation *R) const {
  llvm_unreachable("getValueForDiscardedRelocations not implemented yet.");
}

/// dynamic - the dynamic section of the target machine.
ELFDynamic *SRRArchLDBackend::dynamic() {
  llvm_unreachable("dynamic not implemented yet.");
}

std::optional<bool>
SRRArchLDBackend::shouldProcessSectionForGC(const ELFSection &pSec) const {
  llvm_unreachable("shouldProcessSectionForGC not implemented yet.");
}

unsigned int
SRRArchLDBackend::getTargetSectionOrder(const ELFSection &pSectHdr) const {
  llvm_unreachable("getTargetSectionOrder not implemented yet.");
}

void SRRArchLDBackend::addTargetSpecificSegments() {
  llvm_unreachable("addTargetSpecificSegments not implemented yet.");
}

void SRRArchLDBackend::setDefaultConfigs() {
  GNULDBackend::setDefaultConfigs();
  if (config().options().threadsEnabled() &&
      !config().isGlobalThreadingEnabled()) {
    config().disableThreadOptions(
        LinkerConfig::EnableThreadsOpt::ScanRelocations |
        LinkerConfig::EnableThreadsOpt::ApplyRelocations |
        LinkerConfig::EnableThreadsOpt::LinkerRelaxation);
  }
}

namespace eld {

//===----------------------------------------------------------------------===//
/// createSRRArchLDBackend - the help function to create corresponding
/// SRRArchLDBackend
GNULDBackend *createSRRArchLDBackend(Module &pModule) {
  return make<SRRArchLDBackend>(
      pModule, make<SRRArchStandaloneInfo>(pModule.getConfig()));
}

} // namespace eld

//===----------------------------------------------------------------------===//
// Force static initialization.
//===----------------------------------------------------------------------===//
extern "C" void ELDInitializeSRRArchLDBackend() {
  // Register the linker backend
  eld::TargetRegistry::RegisterGNULDBackend(TheSRRArchTarget,
                                            createSRRArchLDBackend);
}
