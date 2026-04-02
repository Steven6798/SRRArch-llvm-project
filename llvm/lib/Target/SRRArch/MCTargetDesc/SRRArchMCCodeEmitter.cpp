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

  unsigned getBranchTargetOpValue(const MCInst &Inst, unsigned OpNo,
                                  SmallVectorImpl<MCFixup> &Fixups,
                                  const MCSubtargetInfo &SubtargetInfo) const;

  uint64_t getRiMemoryOpValue(const MCInst &Inst, unsigned OpNo,
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

  unsigned Opc = Inst.getOpcode();
  if (Opc == SRRArch::GENINT) {
    Fixups.push_back(MCFixup::create(0, Expr, SRRArch::FIXUP_SRRARCH_GV));
  } else if (Opc == SRRArch::CALL) {
    Fixups.push_back(MCFixup::create(0, Expr, SRRArch::FIXUP_SRRARCH_CALL));
  } else {
    llvm_unreachable("Invalid expresion");
  }
  return 0;
}

unsigned SRRArchMCCodeEmitter::getBranchTargetOpValue(
    const MCInst &Inst, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  const MCOperand &MCOp = Inst.getOperand(OpNo);
  if (MCOp.isReg() || MCOp.isImm())
    return getMachineOpValue(Inst, MCOp, Fixups, SubtargetInfo);

  unsigned kind = SRRArch::FIXUP_SRRARCH_BR;
  if (Inst.getOpcode() == SRRArch::BRCOND)
    kind = SRRArch::FIXUP_SRRARCH_BRCOND;

  Fixups.push_back(MCFixup::create(0, MCOp.getExpr(), kind));

  return 0;
}

uint64_t SRRArchMCCodeEmitter::getRiMemoryOpValue(
    const MCInst &Inst, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  const MCOperand Reg = Inst.getOperand(OpNo);
  const MCOperand Offset = Inst.getOperand(OpNo + 1);

  assert(Reg.isReg() && "First operand is not a register.");
  assert(Offset.isImm() && "Second operand is not an immediate.");

  uint64_t Encoding = 0;
  Encoding |= (getMachineOpValue(Inst, Reg, Fixups, SubtargetInfo) & 0x1F);
  Encoding |= (getMachineOpValue(Inst, Offset, Fixups, SubtargetInfo) & 0xFFF)
              << 5;
  return Encoding;
}

void SRRArchMCCodeEmitter::encodeInstruction(
    const MCInst &Inst, SmallVectorImpl<char> &CB,
    SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  LLVM_DEBUG(dbgs() << "Encoding: " << Inst << "\n");

  // Get instruction encoding and emit it
  uint64_t Value = getBinaryCodeForInstr(Inst, Fixups, SubtargetInfo);
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
