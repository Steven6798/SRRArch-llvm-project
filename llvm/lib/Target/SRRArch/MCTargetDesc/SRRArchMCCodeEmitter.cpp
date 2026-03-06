//===-- SRRArchMCCodeEmitter.cpp - Convert SRRArch code to machine code ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SRRArchMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SRRArchBaseInfo.h"
#include "MCTargetDesc/SRRArchFixupKinds.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInst.h"

#define DEBUG_TYPE "mccodeemitter"

namespace llvm {

namespace {

class SRRArchMCCodeEmitter : public MCCodeEmitter {
public:
  SRRArchMCCodeEmitter(const MCInstrInfo &MCII, MCContext &C) {}
  SRRArchMCCodeEmitter(const SRRArchMCCodeEmitter &) = delete;
  void operator=(const SRRArchMCCodeEmitter &) = delete;
  ~SRRArchMCCodeEmitter() override = default;

  // The functions below are called by TableGen generated functions for getting
  // the binary encoding of instructions/opereands.

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &Inst,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &SubtargetInfo) const;

  // getMachineOpValue - Return binary encoding of operand. If the machine
  // operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &Inst, const MCOperand &MCOp,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &SubtargetInfo) const;

  void encodeInstruction(const MCInst &Inst, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &SubtargetInfo) const override;
};

} // end anonymous namespace

// getMachineOpValue - Return binary encoding of operand. If the machine
// operand requires relocation, record the relocation and return zero.
unsigned SRRArchMCCodeEmitter::getMachineOpValue(
    const MCInst &Inst, const MCOperand &MCOp, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  llvm_unreachable("getMachineOpValue not implemented yet");
  return 0;
}

void SRRArchMCCodeEmitter::encodeInstruction(
    const MCInst &Inst, SmallVectorImpl<char> &CB,
    SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  llvm_unreachable("encodeInstruction not implemented yet");
}

#include "SRRArchGenMCCodeEmitter.inc"

} // end namespace llvm

llvm::MCCodeEmitter *
llvm::createSRRArchMCCodeEmitter(const MCInstrInfo &InstrInfo,
                                 MCContext &context) {
  return new SRRArchMCCodeEmitter(InstrInfo, context);
}
