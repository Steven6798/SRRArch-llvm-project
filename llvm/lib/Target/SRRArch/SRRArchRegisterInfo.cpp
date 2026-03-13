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
#include "llvm/Support/Debug.h"

#define GET_REGINFO_TARGET_DESC
#include "SRRArchGenRegisterInfo.inc"

using namespace llvm;

#define DEBUG_TYPE "srrarch-register-info"

SRRArchRegisterInfo::SRRArchRegisterInfo()
    : SRRArchGenRegisterInfo(/*RA=*/SRRArch::R4, 0, 0, /*PC=*/SRRArch::R0) {
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
  Reserved.set(SRRArch::R8);
  return Reserved;
}

bool SRRArchRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MI.getParent()->getParent();
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  bool HasFP = TFI->hasFP(MF);
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // Addressable stack objects are addressed using neg. offsets from fp
  // or pos. offsets from sp/basepointer
  if (!HasFP || (hasStackRealignment(MF) && FrameIndex >= 0))
    Offset += MF.getFrameInfo().getStackSize();

  Register FrameReg = getFrameRegister(MF);
  if (FrameIndex >= 0 && hasStackRealignment(MF)) {
    FrameReg = getStackRegister();
  }

  // Replace frame index with a frame pointer reference.
  unsigned Opc = MI.getOpcode();
  if (Opc == SRRArch::LOAD || Opc == SRRArch::STORE) {
    assert(RS && "Register scavenging must be on");

    RS->enterBasicBlockEnd(MBB);
    RS->backward(std::next(II));

    Register ScratchReg = RS->scavengeRegisterBackwards(
        SRRArch::GPRRegClass, II, /*RestoreAfter=*/false, /*SPAdj=*/0,
        /*AllowSpill=*/true);
    assert(ScratchReg != 0 && "scratch reg was 0");
    RS->setRegUsed(ScratchReg);

    bool HasNegOffset = false;
    if (Offset < 0) {
      HasNegOffset = true;
      Offset = -Offset;
    }

    BuildMI(MBB, II, DL, TII->get(SRRArch::GENINT), ScratchReg).addImm(Offset);

    unsigned Opc = HasNegOffset ? SRRArch::SUB : SRRArch::ADD;
    BuildMI(MBB, II, DL, TII->get(Opc), ScratchReg)
        .addReg(FrameReg)
        .addReg(ScratchReg);

    MI.getOperand(FIOperandNum)
        .ChangeToRegister(ScratchReg, /*isDef=*/false, /*isImp=*/false,
                          /*isKill=*/true);
    return false;
  }

  llvm_unreachable("Unexpected opcode in frame index operation");
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
