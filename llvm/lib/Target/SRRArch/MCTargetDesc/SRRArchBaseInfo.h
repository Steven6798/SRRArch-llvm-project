//===-- SRRArchBaseInfo.h - Top level definitions for SRRArch MC *- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the SRRArch target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHBASEINFO_H
#define LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHBASEINFO_H

#include "SRRArchMCTargetDesc.h"

namespace llvm {

// SRRArchII - This namespace holds all of the target specific flags that
// instruction info tracks.
namespace SRRArchII {
// Target Operand Flag enum.
enum TOF {
  //===------------------------------------------------------------------===//
  // SRRArch Specific MachineOperand flags.
  MO_NO_FLAG,
};
} // namespace SRRArchII

} // namespace llvm
#endif // LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHBASEINFO_H
