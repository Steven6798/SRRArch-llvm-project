//===-- SRRArchTargetMachine.cpp - Define TargetMachine for SRRArch -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about SRRArch target spec.
//
//===----------------------------------------------------------------------===//

#include "SRRArchTargetMachine.h"

#include "SRRArch.h"
#include "SRRArchMachineFunctionInfo.h"
#include "SRRArchTargetObjectFile.h"
#include "SRRArchTargetTransformInfo.h"
#include "TargetInfo/SRRArchTargetInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>

using namespace llvm;

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeSRRArchTarget() {
  // Register the target.
  RegisterTargetMachine<SRRArchTargetMachine> registered_target(
      getTheSRRArchTarget());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeSRRArchAsmPrinterPass(PR);
  initializeSRRArchDAGToDAGISelLegacyPass(PR);
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

SRRArchTargetMachine::SRRArchTargetMachine(
    const Target &T, const Triple &TT, StringRef Cpu, StringRef FeatureString,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CodeModel, CodeGenOptLevel OptLevel,
    bool JIT)
    : CodeGenTargetMachineImpl(
          T, TT.computeDataLayout(), TT, Cpu, FeatureString, Options,
          getEffectiveRelocModel(RM),
          getEffectiveCodeModel(CodeModel, CodeModel::Medium), OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel),
      TLOF(new SRRArchTargetObjectFile()) {
  initAsmInfo();
}

TargetTransformInfo
SRRArchTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(std::make_unique<SRRArchTTIImpl>(this, F));
}

MachineFunctionInfo *SRRArchTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return SRRArchMachineFunctionInfo::create<SRRArchMachineFunctionInfo>(
      Allocator, F, STI);
}

namespace {
// SRRArch Code Generator Pass Configuration Options.
class SRRArchPassConfig : public TargetPassConfig {
public:
  SRRArchPassConfig(SRRArchTargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  SRRArchTargetMachine &getSRRArchTargetMachine() const {
    return getTM<SRRArchTargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *
SRRArchTargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new SRRArchPassConfig(*this, &PassManager);
}

// Install an instruction selector pass.
bool SRRArchPassConfig::addInstSelector() {
  addPass(createSRRArchISelDag(getSRRArchTargetMachine()));
  return false;
}
