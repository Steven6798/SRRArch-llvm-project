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
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include <cassert>

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace llvm {

namespace {

class SRRArchMCCodeEmitter : public MCCodeEmitter {
  MCContext &Ctx;

public:
  SRRArchMCCodeEmitter(const MCInstrInfo &MCII, MCContext &ctx) : Ctx(ctx) {}
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
  if (MCOp.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MCOp.getReg());

  if (MCOp.isImm())
    return MCOp.getImm();

  // MCOp must be an expression
  assert(MCOp.isExpr());
  const MCExpr *Expr = MCOp.getExpr();

  // Extract the symbolic reference side of a binary expression.
  if (Expr->getKind() == MCExpr::Binary) {
    const MCBinaryExpr *BinaryExpr = static_cast<const MCBinaryExpr *>(Expr);
    Expr = BinaryExpr->getLHS();
  }

  if (Inst.getOpcode() == SRRArch::GENINT) {
    Fixups.push_back(MCFixup::create(0, Expr, SRRArch::FIXUP_SRRARCH_GV));
  } else {
    llvm_unreachable("Invalid expresion");
  }
  return 0;
}

void SRRArchMCCodeEmitter::encodeInstruction(
    const MCInst &Inst, SmallVectorImpl<char> &CB,
    SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  LLVM_DEBUG(dbgs() << "Encoding: " << Inst << "\n");

  // Get instruction encoding and emit it
  unsigned Value = getBinaryCodeForInstr(Inst, Fixups, SubtargetInfo);
  ++MCNumEmitted; // Keep track of the number of emitted insns.

  support::endian::write<uint64_t>(CB, Value, llvm::endianness::little);
}

#include "SRRArchGenMCCodeEmitter.inc"

} // end namespace llvm

llvm::MCCodeEmitter *
llvm::createSRRArchMCCodeEmitter(const MCInstrInfo &InstrInfo,
                                 MCContext &context) {
  return new SRRArchMCCodeEmitter(InstrInfo, context);
}
