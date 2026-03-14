//===- SRRArchDisassembler.cpp - Disassembler for SRRArch --------*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is part of the SRRArch Disassembler.
//
//===----------------------------------------------------------------------===//

#include "SRRArchDisassembler.h"
#include "SRRArchInstrInfo.h"
#include "TargetInfo/SRRArchTargetInfo.h"
#include "llvm/MC/MCDecoder.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/TargetRegistry.h"

#define DEBUG_TYPE "srrarch-disassembler"

using namespace llvm;
using namespace llvm::MCD;

typedef MCDisassembler::DecodeStatus DecodeStatus;

static MCDisassembler *createSRRArchDisassembler(const Target & /*T*/,
                                                 const MCSubtargetInfo &STI,
                                                 MCContext &Ctx) {
  return new SRRArchDisassembler(STI, Ctx);
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeSRRArchDisassembler() {
  // Register the disassembler
  TargetRegistry::RegisterMCDisassembler(getTheSRRArchTarget(),
                                         createSRRArchDisassembler);
}

SRRArchDisassembler::SRRArchDisassembler(const MCSubtargetInfo &STI,
                                         MCContext &Ctx)
    : MCDisassembler(STI, Ctx) {}

static const unsigned GPRDecoderTable[] = {
    SRRArch::R0,  SRRArch::R1,  SRRArch::R2,  SRRArch::R3,  SRRArch::R4,
    SRRArch::R5,  SRRArch::R6,  SRRArch::R7,  SRRArch::R8,  SRRArch::R9,
    SRRArch::R10, SRRArch::R11, SRRArch::R12, SRRArch::R13, SRRArch::R14,
    SRRArch::R15, SRRArch::R16, SRRArch::R17, SRRArch::R18, SRRArch::R19,
    SRRArch::R20, SRRArch::R21, SRRArch::R22, SRRArch::R23, SRRArch::R24,
    SRRArch::R25, SRRArch::R26, SRRArch::R27, SRRArch::R28, SRRArch::R29,
    SRRArch::R30, SRRArch::R31};

static DecodeStatus DecodeGPRRegisterClass(MCInst &Inst, unsigned RegNo,
                                           uint64_t /*Address*/,
                                           const MCDisassembler * /*Decoder*/) {
  if (RegNo > 31)
    return MCDisassembler::Fail;

  unsigned Reg = GPRDecoderTable[RegNo];
  Inst.addOperand(MCOperand::createReg(Reg));
  return MCDisassembler::Success;
}

static DecodeStatus decodeBranch(MCInst &MI, unsigned Insn, uint64_t Address,
                                 const MCDisassembler *Decoder) {
  if (Decoder->tryAddingSymbolicOperand(MI, Insn + Address, Address, false, 13,
                                        32, /*InstSize=*/0))
    MI.addOperand(MCOperand::createImm(Insn));
  return MCDisassembler::Success;
}

#include "SRRArchGenDisassemblerTables.inc"

DecodeStatus
SRRArchDisassembler::getInstruction(MCInst &Instr, uint64_t &Size,
                                    ArrayRef<uint8_t> Bytes, uint64_t Address,
                                    raw_ostream & /*CStream*/) const {
  Size = 0;
  // We want to read exactly 8 bytes of data.
  if (Bytes.size() < 8)
    return MCDisassembler::Fail;

  Size = 8;
  // Encoded as a little-endian 64-bit word in the stream.
  uint64_t Insn = (uint64_t(Bytes[7]) << 56) | (uint64_t(Bytes[6]) << 48) |
                  (uint64_t(Bytes[5]) << 40) | (uint64_t(Bytes[4]) << 32) |
                  (uint64_t(Bytes[3]) << 24) | (uint64_t(Bytes[2]) << 16) |
                  (uint64_t(Bytes[1]) << 8) | (uint64_t(Bytes[0]) << 0);

  // Call auto-generated decoder function
  DecodeStatus Result =
      decodeInstruction(DecoderTableSRRArch64, Instr, Insn, Address, this, STI);

  if (Result != MCDisassembler::Fail) {
    Size = 8;
    return Result;
  }

  return MCDisassembler::Fail;
}
