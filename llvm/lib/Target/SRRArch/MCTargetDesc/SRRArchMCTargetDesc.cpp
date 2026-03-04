//===-- SRRArchMCTargetDesc.cpp - SRRArch Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides SRRArch specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "SRRArchMCTargetDesc.h"
#include "SRRArchInstPrinter.h"
#include "SRRArchMCAsmInfo.h"
#include "TargetInfo/SRRArchTargetInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/TargetParser/Triple.h"
#include <cstdint>
#include <string>

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "SRRArchGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SRRArchGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SRRArchGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createSRRArchMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSRRArchMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createSRRArchMCRegisterInfo(const Triple & /*TT*/) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSRRArchMCRegisterInfo(X, SRRArch::RCA, 0, 0, SRRArch::PC);
  return X;
}

static MCSubtargetInfo *
createSRRArchMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string CPUName = std::string(CPU);
  if (CPUName.empty())
    CPUName = "generic";

  return createSRRArchMCSubtargetInfoImpl(TT, CPUName, /*TuneCPU*/ CPUName, FS);
}

static MCStreamer *createMCStreamer(const Triple &T, MCContext &Context,
                                    std::unique_ptr<MCAsmBackend> &&MAB,
                                    std::unique_ptr<MCObjectWriter> &&OW,
                                    std::unique_ptr<MCCodeEmitter> &&Emitter) {
  if (!T.isOSBinFormatELF())
    llvm_unreachable("OS not supported");

  return createELFStreamer(Context, std::move(MAB), std::move(OW),
                           std::move(Emitter));
}

static MCInstPrinter *createSRRArchMCInstPrinter(const Triple & /*T*/,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new SRRArchInstPrinter(MAI, MII, MRI);
  return nullptr;
}

static MCRelocationInfo *createSRRArchElfRelocation(const Triple &TheTriple,
                                                  MCContext &Ctx) {
  return createMCRelocationInfo(TheTriple, Ctx);
}

namespace {

class SRRArchMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit SRRArchMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    if (Inst.getNumOperands() == 0)
      return false;
    if (!isConditionalBranch(Inst) && !isUnconditionalBranch(Inst) &&
        !isCall(Inst))
      return false;

    if (Info->get(Inst.getOpcode()).operands()[0].OperandType ==
        MCOI::OPERAND_PCREL) {
      int64_t Imm = Inst.getOperand(0).getImm();
      Target = Addr + Size + Imm;
      return true;
    } else {
      int64_t Imm = Inst.getOperand(0).getImm();

      // Skip case where immediate is 0 as that occurs in file that isn't linked
      // and the branch target inferred would be wrong.
      if (Imm == 0)
        return false;

      Target = Imm;
      return true;
    }
  }
};

} // end anonymous namespace

static MCInstrAnalysis *createSRRArchInstrAnalysis(const MCInstrInfo *Info) {
  return new SRRArchMCInstrAnalysis(Info);
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeSRRArchTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<SRRArchMCAsmInfo> X(getTheSRRArchTarget());

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getTheSRRArchTarget(),
                                      createSRRArchMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getTheSRRArchTarget(),
                                    createSRRArchMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getTheSRRArchTarget(),
                                          createSRRArchMCSubtargetInfo);

  // Register the MC code emitter
  TargetRegistry::RegisterMCCodeEmitter(getTheSRRArchTarget(),
                                        createSRRArchMCCodeEmitter);

  // Register the ASM Backend
  TargetRegistry::RegisterMCAsmBackend(getTheSRRArchTarget(),
                                       createSRRArchAsmBackend);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheSRRArchTarget(),
                                        createSRRArchMCInstPrinter);

  // Register the ELF streamer.
  TargetRegistry::RegisterELFStreamer(getTheSRRArchTarget(), createMCStreamer);

  // Register the MC relocation info.
  TargetRegistry::RegisterMCRelocationInfo(getTheSRRArchTarget(),
                                           createSRRArchElfRelocation);

  // Register the MC instruction analyzer.
  TargetRegistry::RegisterMCInstrAnalysis(getTheSRRArchTarget(),
                                          createSRRArchInstrAnalysis);
}
