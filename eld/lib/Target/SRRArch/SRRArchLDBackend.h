//===-SRRArchLDBackend.h---------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
#ifndef SRRARCH_LDBACKEND_H
#define SRRARCH_LDBACKEND_H

#include "SRRArchGOT.h"
#include "eld/Config/LinkerConfig.h"
#include "eld/Fragment/RegionFragmentEx.h"
#include "eld/Object/ObjectBuilder.h"
#include "eld/Readers/ELFSection.h"
#include "eld/SymbolResolver/IRBuilder.h"
#include "eld/Target/GNULDBackend.h"
#include <unordered_set>

namespace eld {

class LinkerConfig;
class SRRArchInfo;
class SRRArchAttributeFragment;
class SRRArchELFDynamic;
class SRRArchPLT;

//===----------------------------------------------------------------------===//
/// SRRArchLDBackend - linker backend of SRRArch target of GNU ELF format
///
class SRRArchLDBackend : public GNULDBackend {
public:
  SRRArchLDBackend(Module &pModule, SRRArchInfo *pInfo);

  ~SRRArchLDBackend();

  void initializeAttributes() override;

  /// initRelocator - create and initialize Relocator.
  bool initRelocator() override;

  /// getRelocator - return relocator.
  Relocator *getRelocator() const override;

  void initTargetSections(ObjectBuilder &pBuilder) override;

  void initDynamicSections(ELFObjectFile &) override;

  void initPatchSections(ELFObjectFile &) override;

  void initTargetSymbols() override;

  bool initBRIslandFactory() override;

  bool initStubFactory() override;

  void mayBeRelax(int pass, bool &pFinished) override;

  /// getTargetSectionOrder - compute the layout order of SRRArch target section
  unsigned int getTargetSectionOrder(const ELFSection &pSectHdr) const override;

  /// finalizeTargetSymbols - finalize the symbol value
  bool finalizeTargetSymbols() override;

  ELFDynamic *dynamic() override;

  void evaluateTargetSymbolsBeforeRelaxation() override;

  Stub *getBranchIslandStub(Relocation *pReloc,
                            int64_t pTargetValue) const override {
    return nullptr;
  }

  bool validateArchOpts() const override;

  void doPreLayout() override;

  bool handleRelocation(ELFSection *pSection, Relocation::Type pType,
                        LDSymbol &pSym, uint32_t pOffset,
                        Relocation::Address pAddend = 0,
                        bool pLastPass = false) override;

  virtual bool readSection(InputFile &pInput, ELFSection *S) override;

  bool shouldIgnoreRelocSync(Relocation *pReloc) const override;

  Relocation::Type
      getRemappedInternalRelocationType(Relocation::Type) const override;

  Relocation::Type getCopyRelType() const override;

  uint64_t getValueForDiscardedRelocations(const Relocation *R) const override;

  // ----------------------- GC override ----------------------------
  std::optional<bool>
  shouldProcessSectionForGC(const ELFSection &pSec) const override;

  // ---------------------  PLT Support ---------------------------

  // ---------------------  Dynamic relocation support ------------
  bool hasSymInfo(const Relocation *X) const override {
    llvm_unreachable("hasSymInfo not implemented yet.");
    return true;
  }

  DynRelocType getDynRelocType(const Relocation *X) const override {
    llvm_unreachable("hasSymInfo not implemented yet.");
    return DynRelocType::DEFAULT;
  }

  std::size_t PLTEntriesCount() const override {
    llvm_unreachable("PLTEntriesCount not implemented yet.");
  }

  std::size_t GOTEntriesCount() const override {
    llvm_unreachable("GOTEntriesCount not implemented yet.");
  }

  void doCreateProgramHdrs() override { return; }

  void addTargetSpecificSegments() override;

  void setDefaultConfigs() override;

private:
  /// getRelEntrySize - the size in BYTE of rela type relocation
  size_t getRelEntrySize() override {
    llvm_unreachable("getRelEntrySize not implemented yet.");
  }

  /// getRelaEntrySize - the size in BYTE of rela type relocation
  size_t getRelaEntrySize() override {
    llvm_unreachable("getRelaEntrySize not implemented yet.");
  }

  uint64_t maxBranchOffset() override {
    llvm_unreachable("maxBranchOffset not implemented yet.");
  }

  bool finalizeScanRelocations() override;

  ELFSection *mergeSection(ELFSection *pSection) override;

  /// postProcessing - Backend can do any needed modification in the final stage
  eld::Expected<void> postProcessing(llvm::FileOutputBuffer &pOutput) override;

private:
  Relocator *m_pRelocator;
  LDSymbol *m_pEndOfImage;
};
} // namespace eld

#endif
