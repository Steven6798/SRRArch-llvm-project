//===- SRRArchDisassembler.cpp - Disassembler for SRRArch -----------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_SRRARCH_DISASSEMBLER_SRRARCHDISASSEMBLER_H
#define LLVM_LIB_TARGET_SRRARCH_DISASSEMBLER_SRRARCHDISASSEMBLER_H

#include "llvm/MC/MCDisassembler/MCDisassembler.h"

namespace llvm {

class SRRArchDisassembler : public MCDisassembler {
public:
  SRRArchDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx);

  ~SRRArchDisassembler() override = default;

  // getInstruction - See MCDisassembler.
  MCDisassembler::DecodeStatus
  getInstruction(MCInst &Instr, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                 uint64_t Address, raw_ostream &CStream) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_SRRARCH_DISASSEMBLER_SRRARCHDISASSEMBLER_H
