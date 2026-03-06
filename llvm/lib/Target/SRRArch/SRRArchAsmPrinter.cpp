//===-- SRRArchAsmPrinter.cpp - SRRArch LLVM assembly writer --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the SRRArch assembly language.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SRRArchInstPrinter.h"
#include "SRRArchMCInstLower.h"
#include "SRRArchTargetMachine.h"
#include "TargetInfo/SRRArchTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

#define DEBUG_TYPE "asm-printer"

using namespace llvm;

namespace {
class SRRArchAsmPrinter : public AsmPrinter {
public:
  explicit SRRArchAsmPrinter(TargetMachine &TM,
                             std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer), ID) {}

  StringRef getPassName() const override { return "SRRArch Assembly Printer"; }

  void printOperand(const MachineInstr *MI, int OpNum, raw_ostream &O);
  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       const char *ExtraCode, raw_ostream &O) override;
  void emitInstruction(const MachineInstr *MI) override;
  bool isBlockOnlyReachableByFallthrough(
      const MachineBasicBlock *MBB) const override;

public:
  static char ID;
};
} // end of anonymous namespace

void SRRArchAsmPrinter::printOperand(const MachineInstr *MI, int OpNum,
                                     raw_ostream &O) {
  llvm_unreachable("printOperand not implemented yet");
}

// PrintAsmOperand - Print out an operand for an inline asm expression.
bool SRRArchAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                        const char *ExtraCode, raw_ostream &O) {
  llvm_unreachable("PrintAsmOperand not implemented yet");
  return false;
}

//===----------------------------------------------------------------------===//
void SRRArchAsmPrinter::emitInstruction(const MachineInstr *MI) {
  llvm_unreachable("emitInstruction not implemented yet");
}

// isBlockOnlyReachableByFallthough - Return true if the basic block has
// exactly one predecessor and the control transfer mechanism between
// the predecessor and this block is a fall-through.
bool SRRArchAsmPrinter::isBlockOnlyReachableByFallthrough(
    const MachineBasicBlock *MBB) const {
  llvm_unreachable("isBlockOnlyReachableByFallthrough not implemented yet");
  return false;
}

char SRRArchAsmPrinter::ID = 0;

INITIALIZE_PASS(SRRArchAsmPrinter, "srrarch-asm-printer",
                "SRRArch Assembly Printer", false, false)

// Force static initialization.
extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeSRRArchAsmPrinter() {
  RegisterAsmPrinter<SRRArchAsmPrinter> X(getTheSRRArchTarget());
}
