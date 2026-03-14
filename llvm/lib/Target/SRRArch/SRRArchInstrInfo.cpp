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
    : SRRArchGenInstrInfo(STI, RegisterInfo, SRRArch::ADJCALLSTACKDOWN,
                          SRRArch::ADJCALLSTACKUP),
      RegisterInfo() {}

void SRRArchInstrInfo::copyPhysReg(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
    const DebugLoc &DL, Register DestinationRegister, Register SourceRegister,
    bool KillSource, bool RenamableDest, bool RenamableSrc) const {
  BuildMI(MBB, Position, DL, get(SRRArch::MOV), DestinationRegister)
      .addReg(SourceRegister, getKillRegState(KillSource));
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
  // Iterator to current instruction being considered.
  MachineBasicBlock::iterator Instruction = MBB.end();

  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  while (Instruction != MBB.begin()) {
    --Instruction;

    // Skip over debug instructions.
    if (Instruction->isDebugInstr())
      continue;

    // Working from the bottom, when we see a non-terminator
    // instruction, we're done.
    if (!isUnpredicatedTerminator(*Instruction))
      break;

    // A terminator that isn't a branch can't easily be handled
    // by this analysis.
    if (!Instruction->isBranch())
      return true;

    // Handle unconditional branches.
    unsigned Opcode = Instruction->getOpcode();
    if (Opcode == SRRArch::BR) {
      if (!AllowModify) {
        TrueBlock = Instruction->getOperand(0).getMBB();
        continue;
      }

      // If the block has any instructions after a branch, delete them.
      MBB.erase(std::next(Instruction), MBB.end());

      Condition.clear();
      FalseBlock = nullptr;

      // Delete the jump if it's equivalent to a fall-through.
      if (MBB.isLayoutSuccessor(Instruction->getOperand(0).getMBB())) {
        TrueBlock = nullptr;
        Instruction->eraseFromParent();
        Instruction = MBB.end();
        continue;
      }

      // TrueBlock is used to indicate the unconditional destination.
      TrueBlock = Instruction->getOperand(0).getMBB();
      continue;
    }

    // Handle conditional branches
    if (Opcode != SRRArch::BRCOND)
      return true; // Unknown opcode.

    // Multiple conditional branches are not handled here so only proceed if
    // there are no conditions enqueued.
    if (Condition.empty()) {
      // TrueBlock is the target of the previously seen unconditional branch.
      FalseBlock = TrueBlock;
      TrueBlock = Instruction->getOperand(1).getMBB();
      Condition.push_back(Instruction->getOperand(0));
      continue;
    }

    // Multiple conditional branches are not handled.
    return true;
  }

  // Return false indicating branch successfully analyzed.
  return false;
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
