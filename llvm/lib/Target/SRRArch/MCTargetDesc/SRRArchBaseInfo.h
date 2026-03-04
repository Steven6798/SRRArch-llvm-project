//===-- SRRArchBaseInfo.h - Top level definitions for SRRArch MC ----*- C++ -*-===//
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
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

// SRRArchII - This namespace holds all of the target specific flags that
// instruction info tracks.
namespace SRRArchII {
// Target Operand Flag enum.
enum TOF {
  //===------------------------------------------------------------------===//
  // SRRArch Specific MachineOperand flags.
  MO_NO_FLAG,

  // MO_ABS_HI/LO - Represents the hi or low part of an absolute symbol
  // address.
  MO_ABS_HI,
  MO_ABS_LO,
};
} // namespace SRRArchII

static inline unsigned getSRRArchRegisterNumbering(MCRegister Reg) {
  switch (Reg.id()) {
  case SRRArch::R0:
    return 0;
  case SRRArch::R1:
    return 1;
  case SRRArch::R2:
  case SRRArch::PC:
    return 2;
  case SRRArch::R3:
    return 3;
  case SRRArch::R4:
  case SRRArch::SP:
    return 4;
  case SRRArch::R5:
  case SRRArch::FP:
    return 5;
  case SRRArch::R6:
    return 6;
  case SRRArch::R7:
    return 7;
  case SRRArch::R8:
  case SRRArch::RV:
    return 8;
  case SRRArch::R9:
    return 9;
  case SRRArch::R10:
  case SRRArch::RR1:
    return 10;
  case SRRArch::R11:
  case SRRArch::RR2:
    return 11;
  case SRRArch::R12:
    return 12;
  case SRRArch::R13:
    return 13;
  case SRRArch::R14:
    return 14;
  case SRRArch::R15:
  case SRRArch::RCA:
    return 15;
  case SRRArch::R16:
    return 16;
  case SRRArch::R17:
    return 17;
  case SRRArch::R18:
    return 18;
  case SRRArch::R19:
    return 19;
  case SRRArch::R20:
    return 20;
  case SRRArch::R21:
    return 21;
  case SRRArch::R22:
    return 22;
  case SRRArch::R23:
    return 23;
  case SRRArch::R24:
    return 24;
  case SRRArch::R25:
    return 25;
  case SRRArch::R26:
    return 26;
  case SRRArch::R27:
    return 27;
  case SRRArch::R28:
    return 28;
  case SRRArch::R29:
    return 29;
  case SRRArch::R30:
    return 30;
  case SRRArch::R31:
    return 31;
  default:
    llvm_unreachable("Unknown register number!");
  }
}
} // namespace llvm
#endif // LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHBASEINFO_H
