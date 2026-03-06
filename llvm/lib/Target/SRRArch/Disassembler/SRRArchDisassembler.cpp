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

#include "SRRArchGenDisassemblerTables.inc"

DecodeStatus
SRRArchDisassembler::getInstruction(MCInst &Instr, uint64_t &Size,
                                    ArrayRef<uint8_t> Bytes, uint64_t Address,
                                    raw_ostream & /*CStream*/) const {
  llvm_unreachable("getInstruction not implemented yet");

  return MCDisassembler::Fail;
}
