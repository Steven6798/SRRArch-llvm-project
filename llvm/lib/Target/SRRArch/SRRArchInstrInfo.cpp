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

#define DEBUG_TYPE "srrarch-instr-info"

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
  DebugLoc DL;
  if (Position != MBB.end())
    DL = Position->getDebugLoc();

  if (!SRRArch::GPRRegClass.hasSubClassEq(RegisterClass))
    llvm_unreachable("Can't store this register to stack slot");

  MachineInstr *Store = BuildMI(MBB, Position, DL, get(SRRArch::STORE))
                            .addReg(SourceRegister, getKillRegState(IsKill))
                            .addFrameIndex(FrameIndex)
                            .addImm(0);

  LLVM_DEBUG(dbgs() << "Inserting store to stack slot:" << *Store << "\n");
}

void SRRArchInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
    Register DestinationRegister, int FrameIndex,
    const TargetRegisterClass *RegisterClass, Register /*VReg*/,
    unsigned /*SubReg*/, MachineInstr::MIFlag /*Flags*/) const {
  DebugLoc DL;
  if (Position != MBB.end())
    DL = Position->getDebugLoc();

  if (!SRRArch::GPRRegClass.hasSubClassEq(RegisterClass))
    llvm_unreachable("Can't load this register from stack slot");

  MachineInstr *Load =
      BuildMI(MBB, Position, DL, get(SRRArch::LOAD), DestinationRegister)
          .addFrameIndex(FrameIndex)
          .addImm(0);

  LLVM_DEBUG(dbgs() << "Inserting load from stack slot:" << *Load << "\n");
}

bool SRRArchInstrInfo::areMemAccessesTriviallyDisjoint(
    const MachineInstr &MIa, const MachineInstr &MIb) const {
  assert(MIa.mayLoadOrStore() && "MIa must be a load or store.");
  assert(MIb.mayLoadOrStore() && "MIb must be a load or store.");

  if (MIa.hasUnmodeledSideEffects() || MIb.hasUnmodeledSideEffects() ||
      MIa.hasOrderedMemoryRef() || MIb.hasOrderedMemoryRef())
    return false;

  // Retrieve the base register, offset from the base register and width. Width
  // is the size of memory that is being loaded/stored (e.g. 1, 2, 4, 8).  If
  // base registers are identical, and the offset of a lower memory access +
  // the width doesn't overlap the offset of a higher memory access,
  // then the memory accesses are different.
  const TargetRegisterInfo *TRI = &getRegisterInfo();
  const MachineOperand *BaseOpA = nullptr, *BaseOpB = nullptr;
  int64_t OffsetA = 0, OffsetB = 0;
  LocationSize WidthA = LocationSize::precise(0),
               WidthB = LocationSize::precise(0);
  if (getMemOperandWithOffsetWidth(MIa, BaseOpA, OffsetA, WidthA, TRI) &&
      getMemOperandWithOffsetWidth(MIb, BaseOpB, OffsetB, WidthB, TRI)) {
    if (BaseOpA->isIdenticalTo(*BaseOpB)) {
      int LowOffset = std::min(OffsetA, OffsetB);
      int HighOffset = std::max(OffsetA, OffsetB);
      LocationSize LowWidth = (LowOffset == OffsetA) ? WidthA : WidthB;
      if (LowWidth.hasValue() &&
          LowOffset + (int)LowWidth.getValue() <= HighOffset)
        return true;
    }
  }
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

// SRRArch does not have the condition encoded in the branch. The condition
// can only be reversed if the we are still in SSA form, the register
// is virtual and the branch is the only user of the condition. SInce SRRArch
// does not have a direct opposite of greater/less than instructions, only the
// equal and not equal conditions can be reversed.
bool SRRArchInstrInfo::reverseBranchCondition(
    SmallVectorImpl<llvm::MachineOperand> &Condition) const {
  assert((Condition.size() == 1) &&
         "SRRArch branch conditions should have one component.");
  MachineOperand Cond = Condition[0];
  MachineRegisterInfo &MRI =
      Cond.getParent()->getParent()->getParent()->getRegInfo();
  Register Reg = Condition[0].getReg();
  if (!MRI.isSSA() || !Reg.isVirtual() || !MRI.hasOneUse(Reg))
    return false;

  MachineInstr *Def = MRI.getVRegDef(Reg);
  if (!Def)
    return false;

  unsigned Opc = 0;
  switch (Def->getOpcode()) {
  case SRRArch::CMPNE:
    Opc = SRRArch::CMPEQ;
    break;
  case SRRArch::CMPEQ:
    Opc = SRRArch::CMPNE;
    break;
  default:
    return false;
  }

  Def->setDesc(get(Opc));
  return true;
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
  // Shouldn't be a fall through.
  assert(TrueBlock && "insertBranch must not be told to insert a fallthrough");
  assert(!BytesAdded && "code size not handled");

  // If condition is empty then an unconditional branch is being inserted.
  if (Condition.empty()) {
    assert(!FalseBlock && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, DL, get(SRRArch::BR)).addMBB(TrueBlock);
    return 1;
  }

  // Else a conditional branch is inserted.
  assert((Condition.size() == 1) &&
         "SRRArch branch conditions should have one component.");
  BuildMI(&MBB, DL, get(SRRArch::BRCOND))
      .addReg(Condition[0].getReg())
      .addMBB(TrueBlock);

  // If no false block, then false behavior is fall through and no branch needs
  // to be inserted.
  if (!FalseBlock)
    return 1;

  BuildMI(&MBB, DL, get(SRRArch::BR)).addMBB(FalseBlock);
  return 2;
}

unsigned SRRArchInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                        int *BytesRemoved) const {
  assert(!BytesRemoved && "code size not handled");

  MachineBasicBlock::iterator Instruction = MBB.end();
  unsigned Count = 0;

  while (Instruction != MBB.begin()) {
    --Instruction;
    if (Instruction->isDebugInstr())
      continue;
    if (Instruction->getOpcode() != SRRArch::BR &&
        Instruction->getOpcode() != SRRArch::BRCOND) {
      break;
    }

    // Remove the branch.
    Instruction->eraseFromParent();
    Instruction = MBB.end();
    ++Count;
  }

  return Count;
}

bool SRRArchInstrInfo::getMemOperandWithOffsetWidth(
    const MachineInstr &LdSt, const MachineOperand *&BaseOp, int64_t &Offset,
    LocationSize &Width, const TargetRegisterInfo * /*TRI*/) const {
  switch (LdSt.getOpcode()) {
  default:
    return false;
  case SRRArch::LOAD:
  case SRRArch::STORE:
    Width = LocationSize::precise(8);
    break;
  case SRRArch::LOADWS:
  case SRRArch::LOADWZ:
  case SRRArch::STOREW:
    Width = LocationSize::precise(4);
    break;
  case SRRArch::LOADHS:
  case SRRArch::LOADHZ:
  case SRRArch::STOREH:
    Width = LocationSize::precise(2);
    break;
  case SRRArch::LOADBS:
  case SRRArch::LOADBZ:
  case SRRArch::STOREB:
    Width = LocationSize::precise(1);
    break;
  }

  BaseOp = &LdSt.getOperand(1);
  Offset = LdSt.getOperand(2).getImm();

  if (!BaseOp->isReg())
    return false;

  return true;
}

bool SRRArchInstrInfo::getMemOperandsWithOffsetWidth(
    const MachineInstr &LdSt, SmallVectorImpl<const MachineOperand *> &BaseOps,
    int64_t &Offset, bool &OffsetIsScalable, LocationSize &Width,
    const TargetRegisterInfo *TRI) const {
  switch (LdSt.getOpcode()) {
  default:
    return false;
  case SRRArch::LOAD:
  case SRRArch::STORE:
  case SRRArch::LOADWS:
  case SRRArch::LOADWZ:
  case SRRArch::STOREW:
  case SRRArch::LOADHS:
  case SRRArch::LOADHZ:
  case SRRArch::STOREH:
  case SRRArch::LOADBS:
  case SRRArch::LOADBZ:
  case SRRArch::STOREB:
    const MachineOperand *BaseOp;
    OffsetIsScalable = false;
    if (!getMemOperandWithOffsetWidth(LdSt, BaseOp, Offset, Width, TRI))
      return false;
    BaseOps.push_back(BaseOp);
    return true;
  }
}
