//===-- SRRArchFrameLowering.cpp - SRRArch Frame Information --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the SRRArch implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "SRRArchFrameLowering.h"

#include "SRRArchInstrInfo.h"
#include "SRRArchSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

void SRRArchFrameLowering::emitPrologue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {
  llvm_unreachable("emitPrologue not implemented yet");
}

void SRRArchFrameLowering::emitEpilogue(MachineFunction & /*MF*/,
                                        MachineBasicBlock &MBB) const {
  llvm_unreachable("emitEpilogue not implemented yet");
}

void SRRArchFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                                BitVector &SavedRegs,
                                                RegScavenger *RS) const {
  llvm_unreachable("determineCalleeSaves not implemented yet");
}
