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
  llvm_unreachable("getRelocator not implemented yet.");
}

Relocation::Type SRRArchLDBackend::getCopyRelType() const {
  llvm_unreachable("getCopyRelType not implemented yet.");
}

void SRRArchLDBackend::initDynamicSections(ELFObjectFile &InputFile) {
  llvm_unreachable("initDynamicSections not implemented yet.");
}

void SRRArchLDBackend::initTargetSections(ObjectBuilder &pBuilder) {
  llvm_unreachable("initTargetSections not implemented yet.");
}

void SRRArchLDBackend::initPatchSections(ELFObjectFile &InputFile) {
  llvm_unreachable("initPatchSections not implemented yet.");
}

void SRRArchLDBackend::initTargetSymbols() {
  llvm_unreachable("initTargetSymbols not implemented yet.");
}

bool SRRArchLDBackend::initBRIslandFactory() {
  llvm_unreachable("initBRIslandFactory not implemented yet.");
}

bool SRRArchLDBackend::initStubFactory() {
  llvm_unreachable("initStubFactory not implemented yet.");
}

bool SRRArchLDBackend::readSection(InputFile &pInput, ELFSection *S) {
  llvm_unreachable("readSection not implemented yet.");
}

bool SRRArchLDBackend::DoesOverrideMerge(ELFSection *pSection) const {
  llvm_unreachable("DoesOverrideMerge not implemented yet.");
}

ELFSection *SRRArchLDBackend::mergeSection(ELFSection *S) {
  llvm_unreachable("mergeSection not implemented yet.");
}

bool SRRArchLDBackend::addSymbolToOutput(ResolveInfo *Info) {
  llvm_unreachable("addSymbolToOutput not implemented yet.");
}

bool SRRArchLDBackend::shouldIgnoreRelocSync(Relocation *pReloc) const {
  llvm_unreachable("shouldIgnoreRelocSync not implemented yet.");
}

void SRRArchLDBackend::mayBeRelax(int relaxation_pass, bool &pFinished) {
  llvm_unreachable("mayBeRelax not implemented yet.");
}

/// finalizeSymbol - finalize the symbol value
bool SRRArchLDBackend::finalizeTargetSymbols() {
  llvm_unreachable("finalizeTargetSymbols not implemented yet.");
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
  llvm_unreachable("handleRelocation not implemented yet.");
}

bool SRRArchLDBackend::handlePendingRelocations(ELFSection *section) {
  llvm_unreachable("handlePendingRelocations not implemented yet.");
}

Relocation::Type SRRArchLDBackend::getRemappedInternalRelocationType(
    Relocation::Type pType) const {
  llvm_unreachable("getRemappedInternalRelocationType not implemented yet.");
}

void SRRArchLDBackend::doPreLayout() {
  llvm_unreachable("doPreLayout not implemented yet.");
}

void SRRArchLDBackend::evaluateTargetSymbolsBeforeRelaxation() {
  llvm_unreachable(
      "evaluateTargetSymbolsBeforeRelaxation not implemented yet.");
}

bool SRRArchLDBackend::finalizeScanRelocations() {
  llvm_unreachable("finalizeScanRelocations not implemented yet.");
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

/// doCreateProgramHdrs - backend can implement this function to create the
/// target-dependent segments
void SRRArchLDBackend::doCreateProgramHdrs() {
  llvm_unreachable("doCreateProgramHdrs not implemented yet.");
}

int SRRArchLDBackend::numReservedSegments() const {
  llvm_unreachable("numReservedSegments not implemented yet.");
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

eld::Expected<void>
SRRArchLDBackend::postProcessing(llvm::FileOutputBuffer &pOutput) {
  llvm_unreachable("postProcessing not implemented yet.");
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
