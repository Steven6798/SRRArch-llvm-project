//===-- SRRArchInstrInfo.cpp - SRRArch Instruction Information ---*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the SRRArch implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "SRRArchInstrInfo.h"
#include "MCTargetDesc/SRRArchBaseInfo.h"
#include "SRRArchSubtarget.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "SRRArchGenInstrInfo.inc"

SRRArchInstrInfo::SRRArchInstrInfo(const SRRArchSubtarget &STI)
    : SRRArchGenInstrInfo(STI, RegisterInfo), RegisterInfo() {}

void SRRArchInstrInfo::copyPhysReg(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
    const DebugLoc &DL, Register DestinationRegister, Register SourceRegister,
    bool KillSource, bool RenamableDest, bool RenamableSrc) const {
  llvm_unreachable("copyPhysReg not implemented yet");
}

void SRRArchInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
    Register SourceRegister, bool IsKill, int FrameIndex,
    const TargetRegisterClass *RegisterClass, Register /*VReg*/,
    MachineInstr::MIFlag /*Flags*/) const {
  llvm_unreachable("storeRegToStackSlot not implemented yet");
}

void SRRArchInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
    Register DestinationRegister, int FrameIndex,
    const TargetRegisterClass *RegisterClass, Register /*VReg*/,
    unsigned /*SubReg*/, MachineInstr::MIFlag /*Flags*/) const {
  llvm_unreachable("loadRegFromStackSlot not implemented yet");
}

bool SRRArchInstrInfo::areMemAccessesTriviallyDisjoint(
    const MachineInstr &MIa, const MachineInstr &MIb) const {
  llvm_unreachable("areMemAccessesTriviallyDisjoint not implemented yet");
  return false;
}

bool SRRArchInstrInfo::expandPostRAPseudo(MachineInstr & /*MI*/) const {
  llvm_unreachable("expandPostRAPseudo not implemented yet");
  return false;
}

bool SRRArchInstrInfo::analyzeCompare(const MachineInstr &MI, Register &SrcReg,
                                      Register &SrcReg2, int64_t &CmpMask,
                                      int64_t &CmpValue) const {
  llvm_unreachable("analyzeCompare not implemented yet");

  return false;
}

bool SRRArchInstrInfo::optimizeCompareInstr(
    MachineInstr &CmpInstr, Register SrcReg, Register SrcReg2,
    int64_t /*CmpMask*/, int64_t CmpValue,
    const MachineRegisterInfo *MRI) const {
  llvm_unreachable("optimizeCompareInstr not implemented yet");
  return false;
}

bool SRRArchInstrInfo::analyzeSelect(const MachineInstr &MI,
                                     SmallVectorImpl<MachineOperand> &Cond,
                                     unsigned &TrueOp, unsigned &FalseOp,
                                     bool &Optimizable) const {
  llvm_unreachable("analyzeSelect not implemented yet");
  return false;
}

MachineInstr *
SRRArchInstrInfo::optimizeSelect(MachineInstr &MI,
                                 SmallPtrSetImpl<MachineInstr *> &SeenMIs,
                                 bool /*PreferFalse*/) const {
  llvm_unreachable("optimizeSelect not implemented yet");
  return nullptr;
}

// The analyzeBranch function is used to examine conditional instructions and
// remove unnecessary instructions. This method is used by BranchFolder and
// IfConverter machine function passes to improve the CFG.
// - TrueBlock is set to the destination if condition evaluates true (it is the
//   nullptr if the destination is the fall-through branch);
// - FalseBlock is set to the destination if condition evaluates to false (it
//   is the nullptr if the branch is unconditional);
// - condition is populated with machine operands needed to generate the branch
//   to insert in insertBranch;
// Returns: false if branch could successfully be analyzed.
bool SRRArchInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                     MachineBasicBlock *&TrueBlock,
                                     MachineBasicBlock *&FalseBlock,
                                     SmallVectorImpl<MachineOperand> &Condition,
                                     bool AllowModify) const {
  llvm_unreachable("analyzeBranch not implemented yet");

  // Return false indicating branch successfully analyzed.
  return true;
}

// reverseBranchCondition - Reverses the branch condition of the specified
// condition list, returning false on success and true if it cannot be
// reversed.
bool SRRArchInstrInfo::reverseBranchCondition(
    SmallVectorImpl<llvm::MachineOperand> &Condition) const {
  llvm_unreachable("reverseBranchCondition not implemented yet");
  return false;
}

// Insert the branch with condition specified in condition and given targets
// (TrueBlock and FalseBlock). This function returns the number of machine
// instructions inserted.
unsigned SRRArchInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                        MachineBasicBlock *TrueBlock,
                                        MachineBasicBlock *FalseBlock,
                                        ArrayRef<MachineOperand> Condition,
                                        const DebugLoc &DL,
                                        int *BytesAdded) const {
  llvm_unreachable("insertBranch not implemented yet");
  return 0;
}

unsigned SRRArchInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                        int *BytesRemoved) const {
  llvm_unreachable("removeBranch not implemented yet");

  return 0;
}

Register SRRArchInstrInfo::isLoadFromStackSlot(const MachineInstr &MI,
                                               int &FrameIndex) const {
  llvm_unreachable("isLoadFromStackSlot not implemented yet");
  return 0;
}

Register SRRArchInstrInfo::isLoadFromStackSlotPostFE(const MachineInstr &MI,
                                                     int &FrameIndex) const {
  llvm_unreachable("isLoadFromStackSlotPostFE not implemented yet");
  return 0;
}

Register SRRArchInstrInfo::isStoreToStackSlot(const MachineInstr &MI,
                                              int &FrameIndex) const {
  llvm_unreachable("isStoreToStackSlot not implemented yet");
  return 0;
}

bool SRRArchInstrInfo::getMemOperandWithOffsetWidth(
    const MachineInstr &LdSt, const MachineOperand *&BaseOp, int64_t &Offset,
    LocationSize &Width, const TargetRegisterInfo * /*TRI*/) const {
  llvm_unreachable("getMemOperandWithOffsetWidth not implemented yet");
  return false;
}

bool SRRArchInstrInfo::getMemOperandsWithOffsetWidth(
    const MachineInstr &LdSt, SmallVectorImpl<const MachineOperand *> &BaseOps,
    int64_t &Offset, bool &OffsetIsScalable, LocationSize &Width,
    const TargetRegisterInfo *TRI) const {
  llvm_unreachable("getMemOperandsWithOffsetWidth not implemented yet");
  return false;
}
