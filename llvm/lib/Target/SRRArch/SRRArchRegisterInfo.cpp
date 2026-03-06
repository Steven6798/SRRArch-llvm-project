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
#include "llvm/CodeGen/RegisterScavenging.h"

#define GET_REGINFO_TARGET_DESC
#include "SRRArchGenRegisterInfo.inc"

using namespace llvm;

SRRArchRegisterInfo::SRRArchRegisterInfo()
    : SRRArchGenRegisterInfo(SRRArch::R3) {}

const uint16_t *
SRRArchRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  llvm_unreachable("getCalleeSavedRegs not implemented yet");
  return nullptr;
}

BitVector
SRRArchRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  llvm_unreachable("getReservedRegs not implemented yet");
  return Reserved;
}

bool SRRArchRegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const {
  llvm_unreachable("requiresRegisterScavenging not implemented yet");
  return true;
}

bool SRRArchRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  llvm_unreachable("eliminateFrameIndex not implemented yet");
  return false;
}

Register
SRRArchRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  llvm_unreachable("getFrameRegister not implemented yet");
  return 0;
}

Register SRRArchRegisterInfo::getBaseRegister() const {
  llvm_unreachable("getBaseRegister not implemented yet");
  return 0;
}

const uint32_t *
SRRArchRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                          CallingConv::ID /*CC*/) const {
  llvm_unreachable("getCallPreservedMask not implemented yet");
  return nullptr;
}
