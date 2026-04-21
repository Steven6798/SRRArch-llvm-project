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
#include "llvm/CodeGen/RegisterScavenging.h"

using namespace llvm;

#define DEBUG_TYPE "srrarch-frame-lowering"

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
  Register RA = SRI->getRetAddrRegister();

  if (hasFP(MF)) {
    // Allocate space for both RA and FP
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::SUBI), SP)
        .addReg(SP)
        .addImm(16)
        .setMIFlag(MachineInstr::FrameSetup);

    // Save RA at [SP - 8]
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::STORE))
        .addReg(RA)
        .addReg(SP)
        .addImm(8)
        .setMIFlag(MachineInstr::FrameSetup);

    // Save FP at [SP - 16]
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::STORE))
        .addReg(FP)
        .addReg(SP)
        .addImm(0)
        .setMIFlag(MachineInstr::FrameSetup);

    // Set new FP to SP + 16 to point ABOVE the saved RA and FP registers
    BuildMI(MBB, MBBI, DL, SII.get(SRRArch::ADDI), FP)
        .addReg(SP)
        .addImm(16)
        .setMIFlag(MachineInstr::FrameSetup);

    StackSize -= 16; // We already allocated 16 bytes here.
  }

  // Allocate remaining stack space for locals and callee-saved
  if (StackSize > 0) {
    if (isInt<12>(StackSize)) {
      BuildMI(MBB, MBBI, DL, SII.get(SRRArch::SUBI), SP)
          .addReg(SP)
          .addImm(StackSize)
          .setMIFlag(MachineInstr::FrameSetup);
    } else {
      BuildMI(MBB, MBBI, DL, SII.get(SRRArch::GENINT), SRRArch::R9)
          .addImm(StackSize)
          .setMIFlag(MachineInstr::FrameSetup);

      BuildMI(MBB, MBBI, DL, SII.get(SRRArch::SUB), SP)
          .addReg(SP)
          .addReg(SRRArch::R9)
          .setMIFlag(MachineInstr::FrameSetup);
    }
  }

  LLVM_DEBUG(dbgs() << "After Prologue:" << MBB << "\n");
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
  Register RA = SRI->getRetAddrRegister();

  // Restore the stack pointer using the frame pointer value.
  BuildMI(MBB, MBBI, DL, SII.get(SRRArch::MOV), SP)
      .addReg(FP)
      .setMIFlag(MachineInstr::FrameDestroy);

  // Restore the return address (R4) from the stack.
  BuildMI(MBB, MBBI, DL, SII.get(SRRArch::LOAD), RA)
      .addReg(FP)
      .addImm(-8)
      .setMIFlag(MachineInstr::FrameDestroy);

  // Restore the frame pointer from the stack.
  BuildMI(MBB, MBBI, DL, SII.get(SRRArch::LOAD), FP)
      .addReg(FP)
      .addImm(-16)
      .setMIFlag(MachineInstr::FrameDestroy);

  LLVM_DEBUG(dbgs() << "After Epilogue:" << MBB << "\n");
}

void SRRArchFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                                BitVector &SavedRegs,
                                                RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);

  MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
  int Offset = -8;

  // Reserve 8 bytes for the saved RA
  MFI.CreateFixedObject(8, Offset, true);
  Offset -= 8;

  // Reserve 8 bytes for the saved FP
  MFI.CreateFixedObject(8, Offset, true);

  // Create an emergency spill slot for the scavenger.
  // Use the largest register class (GPR) size and alignment.
  const TargetRegisterClass *RC = &SRRArch::GPRRegClass;
  unsigned size = TRI->getSpillSize(*RC);
  Align align = TRI->getSpillAlign(*RC);
  RS->addScavengingFrameIndex(MFI.CreateSpillStackObject(size, align));
}

bool SRRArchFrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return true;

  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
  DebugLoc DL;
  if (MI != MBB.end() && !MI->isDebugInstr())
    DL = MI->getDebugLoc();

  for (auto &CS : CSI) {
    // Insert the spill to the stack frame.
    MCRegister Reg = CS.getReg();
    const TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
    TII.storeRegToStackSlot(MBB, MI, Reg, true, CS.getFrameIdx(), RC,
                            Register());
  }

  return true;
}

bool SRRArchFrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
    MutableArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  DebugLoc DL = (MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc());

  for (auto &CS : CSI) {
    MCRegister Reg = CS.getReg();
    const TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
    TII.loadRegFromStackSlot(MBB, MBBI, Reg, CS.getFrameIdx(), RC, Register());
    assert(MBBI != MBB.begin() &&
           "loadRegFromStackSlot didn't insert any code!");
  }
  return true;
}

MachineBasicBlock::iterator SRRArchFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction & /*MF*/, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  // Discard ADJCALLSTACKDOWN, ADJCALLSTACKUP instructions.
  return MBB.erase(I);
}
