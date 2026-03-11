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

// Determines the size of the frame and maximum call frame size.
void SRRArchFrameLowering::determineFrameLayout(MachineFunction &MF) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const SRRArchRegisterInfo *LRI = STI.getRegisterInfo();

  // Get the number of bytes to allocate from the FrameInfo.
  unsigned FrameSize = MFI.getStackSize();

  // Get the alignment.
  Align StackAlign =
      LRI->hasStackRealignment(MF) ? MFI.getMaxAlign() : getStackAlign();

  // Get the maximum call frame size of all the calls.
  unsigned MaxCallFrameSize = MFI.getMaxCallFrameSize();

  // If we have dynamic alloca then MaxCallFrameSize needs to be aligned so
  // that allocations will be aligned.
  if (MFI.hasVarSizedObjects())
    MaxCallFrameSize = alignTo(MaxCallFrameSize, StackAlign);

  // Update maximum call frame size.
  MFI.setMaxCallFrameSize(MaxCallFrameSize);

  // Include call frame size in total.
  if (!(hasReservedCallFrame(MF) && MFI.adjustsStack()))
    FrameSize += MaxCallFrameSize;

  // Make sure the frame is aligned.
  FrameSize = alignTo(FrameSize, StackAlign);

  // Update frame info.
  MFI.setStackSize(FrameSize);
}

void SRRArchFrameLowering::emitPrologue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const SRRArchInstrInfo &SII = *STI.getInstrInfo();
  const SRRArchRegisterInfo *SRI =
      MF.getSubtarget<SRRArchSubtarget>().getRegisterInfo();
  MachineBasicBlock::iterator MBBI = MBB.begin();

  // Debug location must be unknown since the first debug location is used
  // to determine the end of the prologue.
  DebugLoc DL;

  // Determine the correct frame layout
  determineFrameLayout(MF);

  // FIXME (note copied from Lanai): This appears to be overallocating.  Needs
  // investigation. Get the number of bytes to allocate from the FrameInfo.
  uint64_t StackSize = MFI.getStackSize();

  // Get registers
  Register FP = SRI->getFrameRegister(MF);
  Register SP = SRI->getStackRegister();

  if (hasFP(MF)) {
    // 1. Allocate space for saved FP
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::GENINT), SRRArch::R8)
        .addImm(8)
        .setMIFlag(MachineInstr::FrameSetup);

    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::SUB), SP)
        .addReg(SP)
        .addReg(SRRArch::R8)
        .setMIFlag(MachineInstr::FrameSetup);

    // 2. Save old FP at (sp)
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::STORE))
        .addReg(SP)
        .addReg(FP)
        .setMIFlag(MachineInstr::FrameSetup);

    // 3. Set new FP to current SP
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::MOV), FP)
        .addReg(SP)
        .setMIFlag(MachineInstr::FrameSetup);
  }

  // Allocate remaining stack space for locals and callee-saved
  if (StackSize > 0) {
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::GENINT), SRRArch::R8)
        .addImm(StackSize)
        .setMIFlag(MachineInstr::FrameSetup);

    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::SUB), SP)
        .addReg(SP)
        .addReg(SRRArch::R8)
        .setMIFlag(MachineInstr::FrameSetup);
  }
}

void SRRArchFrameLowering::emitEpilogue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  const SRRArchInstrInfo &SII = *STI.getInstrInfo();
  const SRRArchRegisterInfo *SRI =
      MF.getSubtarget<SRRArchSubtarget>().getRegisterInfo();
  DebugLoc DL = MBBI->getDebugLoc();

  // Get registers
  Register FP = SRI->getFrameRegister(MF);
  Register SP = SRI->getStackRegister();

  // Restore the stack pointer using the callee's frame pointer value.
  BuildMI(MBB, MBBI, DL, SII.get(SRRArch::MOV), SP)
      .addReg(FP)
      .setMIFlag(MachineInstr::FrameDestroy);

  // Restore the frame pointer from the stack.
  BuildMI(MBB, MBBI, DL, SII.get(SRRArch::LOAD), FP)
      .addReg(SP)
      .setMIFlag(MachineInstr::FrameDestroy);
}

void SRRArchFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                                BitVector &SavedRegs,
                                                RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
}
