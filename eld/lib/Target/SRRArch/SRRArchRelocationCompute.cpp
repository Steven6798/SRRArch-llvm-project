//===-SRRArchRelocationCompute.cpp-----------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//
// This  provides functionality to the linker and tools that need to process
// relocations and apply them.
//===----------------------------------------------------------------------===//
#include "SRRArchHelper.h"
#include "SRRArchLLVMExtern.h"
#include "SRRArchRelocationInternal.h"
#include "llvm/Support/LEB128.h"
#include "llvm/Support/MathExtras.h"

namespace {

uint64_t clearImmediateBits(uint64_t Instr, EncodingType Type) {
  // This only has to clear bits in the bytes that are covered by
  // the relocation's Size.
  switch (Type) {
  case EncTy_8:
    return Instr & 0x00;
  case EncTy_16:
    return Instr & 0x0000;
  case EncTy_32:
    return Instr & 0x00000000;
  case EncTy_64:
    return Instr & 0x00000000'00000000;
  case EncTy_None:
    return Instr;
  }
}

// The Relocation helper function that computes the Instruction bits with the
// relocation applied.
template <typename T>
uint64_t doRelocHelper(const RelocationInfo &RelocInfo, uint64_t Instruction,
                       T Value) {
  Instruction = clearImmediateBits(Instruction, RelocInfo.EncType);
  switch (RelocInfo.EncType) {
  case EncTy_64:
    Value = encode64(Value);
    break;
  case EncTy_32:
    Value = encode32(Value);
    break;
  case EncTy_16:
    Value = encode16(Value);
    break;
  case EncTy_8:
    Value = encode8(Value);
    break;
  default:
    break;
  }
  return Value | Instruction;
}
} // anonymous namespace

extern "C" {
uint64_t doRelocSRRArch(const RelocationInfo &RelocInfo, uint64_t Instruction,
                        uint64_t Value) {
  if (SRRArchRelocs[RelocInfo.Type].IsSigned) {
    return doRelocHelper<int64_t>(RelocInfo, Instruction, Value);
  } else {
    return doRelocHelper<uint64_t>(RelocInfo, Instruction, Value);
  }
}
} // extern "C"
