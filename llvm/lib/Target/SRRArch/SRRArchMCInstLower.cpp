//=-- SRRArchMCInstLower.cpp - Convert SRRArch MachineInstr to an MCInst ----=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower SRRArch MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "SRRArchMCInstLower.h"

#include "MCTargetDesc/SRRArchMCAsmInfo.h"

using namespace llvm;

void SRRArchMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  llvm_unreachable("Lower not implemented yet");
}
