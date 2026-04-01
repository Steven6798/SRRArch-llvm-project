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
  llvm_unreachable("applyRelocation not implemented yet.");
}

const char *SRRArchRelocator::getName(Relocation::Type pType) const {
  llvm_unreachable("getName not implemented yet.");
}

SRRArchLDBackend &SRRArchRelocator::getTarget() { return m_Target; }

const SRRArchLDBackend &SRRArchRelocator::getTarget() const { return m_Target; }

void SRRArchRelocator::scanRelocation(Relocation &pReloc,
                                      eld::IRBuilder &pLinker,
                                      ELFSection &pSection,
                                      InputFile &pInputFile,
                                      CopyRelocs &CopyRelocs) {
  llvm_unreachable("scanRelocation not implemented yet.");
}

uint32_t SRRArchRelocator::getNumRelocs() const {
  llvm_unreachable("getNumRelocs not implemented yet.");
}

Relocation::Size SRRArchRelocator::getSize(Relocation::Type pType) const {
  llvm_unreachable("getSize not implemented yet.");
}

void SRRArchRelocator::partialScanRelocation(Relocation &pReloc,
                                             const ELFSection &pSection) {
  llvm_unreachable("partialScanRelocation not implemented yet.");
}

//=========================================//
// Relocation Verifier
//=========================================//

//=========================================//
// Each relocation function implementation //
//=========================================//
// R_SRRARCH_NONE
Relocator::Result eld::none(Relocation &pReloc, SRRArchRelocator &pParent,
                            RelocationDescription &pRelocDesc) {
  return Relocator::OK;
}

Relocator::Result eld::unsupport(Relocation &pReloc, SRRArchRelocator &pParent,
                                 RelocationDescription &pRelocDesc) {
  return SRRArchRelocator::Unsupport;
}
