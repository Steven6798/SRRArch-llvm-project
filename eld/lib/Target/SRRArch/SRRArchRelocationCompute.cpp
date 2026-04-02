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
// The Relocation helper function that computes the Instruction bits with the
// relocation applied.
template <typename T>
uint64_t doRelocHelper(const RelocationInfo &RelocInfo, uint64_t Instruction,
                       T Value) {
  // If the relocation needs the value to be shifted, then lets shift.
  T ValueAfterShift = Value << RelocInfo.Shift;
  return ValueAfterShift | Instruction;
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
