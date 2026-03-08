//===-- SRRArchRegisterInfo.cpp - SRRArch Register Information ---*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the SRRArch implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "SRRArchRegisterInfo.h"
#include "SRRArchFrameLowering.h"
#include "SRRArchInstrInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

#define GET_REGINFO_TARGET_DESC
#include "SRRArchGenRegisterInfo.inc"

using namespace llvm;

SRRArchRegisterInfo::SRRArchRegisterInfo()
    : SRRArchGenRegisterInfo(SRRArch::R3) {
  StackPtr = SRRArch::R1;
  FramePtr = SRRArch::R2;
}

const uint16_t *
SRRArchRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return CSR_SaveList;
}

BitVector
SRRArchRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(SRRArch::R0);
  Reserved.set(SRRArch::R1);
  Reserved.set(SRRArch::R2);
  Reserved.set(SRRArch::R3);
  Reserved.set(SRRArch::R4);
  Reserved.set(SRRArch::R5);
  Reserved.set(SRRArch::R6);
  Reserved.set(SRRArch::R7);
  return Reserved;
}

bool SRRArchRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  llvm_unreachable("eliminateFrameIndex not implemented yet");
  return false;
}

Register
SRRArchRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return FramePtr;
}

const uint32_t *
SRRArchRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                          CallingConv::ID /*CC*/) const {
  llvm_unreachable("getCallPreservedMask not implemented yet");
  return nullptr;
}
