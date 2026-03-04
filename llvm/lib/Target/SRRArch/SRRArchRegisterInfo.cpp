//===-- SRRArchRegisterInfo.cpp - SRRArch Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the SRRArch implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "SRRArchRegisterInfo.h"
#include "SRRArchAluCode.h"
#include "SRRArchCondCode.h"
#include "SRRArchFrameLowering.h"
#include "SRRArchInstrInfo.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "SRRArchGenRegisterInfo.inc"

using namespace llvm;

SRRArchRegisterInfo::SRRArchRegisterInfo() : SRRArchGenRegisterInfo(SRRArch::RCA) {}

const uint16_t *
SRRArchRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return CSR_SaveList;
}

BitVector SRRArchRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(SRRArch::R0);
  Reserved.set(SRRArch::R1);
  Reserved.set(SRRArch::PC);
  Reserved.set(SRRArch::R2);
  Reserved.set(SRRArch::SP);
  Reserved.set(SRRArch::R4);
  Reserved.set(SRRArch::FP);
  Reserved.set(SRRArch::R5);
  Reserved.set(SRRArch::RR1);
  Reserved.set(SRRArch::R10);
  Reserved.set(SRRArch::RR2);
  Reserved.set(SRRArch::R11);
  Reserved.set(SRRArch::RCA);
  Reserved.set(SRRArch::R15);
  if (hasBasePointer(MF))
    Reserved.set(getBaseRegister());
  return Reserved;
}

bool SRRArchRegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const {
  return true;
}

static bool isALUArithLoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case SRRArch::ADD_I_LO:
  case SRRArch::SUB_I_LO:
  case SRRArch::ADD_F_I_LO:
  case SRRArch::SUB_F_I_LO:
  case SRRArch::ADDC_I_LO:
  case SRRArch::SUBB_I_LO:
  case SRRArch::ADDC_F_I_LO:
  case SRRArch::SUBB_F_I_LO:
    return true;
  default:
    return false;
  }
}

static unsigned getOppositeALULoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case SRRArch::ADD_I_LO:
    return SRRArch::SUB_I_LO;
  case SRRArch::SUB_I_LO:
    return SRRArch::ADD_I_LO;
  case SRRArch::ADD_F_I_LO:
    return SRRArch::SUB_F_I_LO;
  case SRRArch::SUB_F_I_LO:
    return SRRArch::ADD_F_I_LO;
  case SRRArch::ADDC_I_LO:
    return SRRArch::SUBB_I_LO;
  case SRRArch::SUBB_I_LO:
    return SRRArch::ADDC_I_LO;
  case SRRArch::ADDC_F_I_LO:
    return SRRArch::SUBB_F_I_LO;
  case SRRArch::SUBB_F_I_LO:
    return SRRArch::ADDC_F_I_LO;
  default:
    llvm_unreachable("Invalid ALU lo opcode");
  }
}

static unsigned getRRMOpcodeVariant(unsigned Opcode) {
  switch (Opcode) {
  case SRRArch::LDBs_RI:
    return SRRArch::LDBs_RR;
  case SRRArch::LDBz_RI:
    return SRRArch::LDBz_RR;
  case SRRArch::LDHs_RI:
    return SRRArch::LDHs_RR;
  case SRRArch::LDHz_RI:
    return SRRArch::LDHz_RR;
  case SRRArch::LDW_RI:
    return SRRArch::LDW_RR;
  case SRRArch::STB_RI:
    return SRRArch::STB_RR;
  case SRRArch::STH_RI:
    return SRRArch::STH_RR;
  case SRRArch::SW_RI:
    return SRRArch::SW_RR;
  default:
    llvm_unreachable("Opcode has no RRM variant");
  }
}

bool SRRArchRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  bool HasFP = TFI->hasFP(MF);
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex) +
               MI.getOperand(FIOperandNum + 1).getImm();

  // Addressable stack objects are addressed using neg. offsets from fp
  // or pos. offsets from sp/basepointer
  if (!HasFP || (hasStackRealignment(MF) && FrameIndex >= 0))
    Offset += MF.getFrameInfo().getStackSize();

  Register FrameReg = getFrameRegister(MF);
  if (FrameIndex >= 0) {
    if (hasBasePointer(MF))
      FrameReg = getBaseRegister();
    else if (hasStackRealignment(MF))
      FrameReg = SRRArch::SP;
  }

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  // Otherwise scavenge a register and encode it into a MOVHI, OR_I_LO sequence.
  if ((isSPLSOpcode(MI.getOpcode()) && !isInt<10>(Offset)) ||
      !isInt<16>(Offset)) {
    assert(RS && "Register scavenging must be on");
    Register Reg = RS->FindUnusedReg(&SRRArch::GPRRegClass);
    if (!Reg)
      Reg = RS->scavengeRegisterBackwards(SRRArch::GPRRegClass, II, false, SPAdj);
    assert(Reg && "Register scavenger failed");

    bool HasNegOffset = false;
    // ALU ops have unsigned immediate values. If the Offset is negative, we
    // negate it here and reverse the opcode later.
    if (Offset < 0) {
      HasNegOffset = true;
      Offset = -Offset;
    }

    if (!isInt<16>(Offset)) {
      // Reg = hi(offset) | lo(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(SRRArch::MOVHI), Reg)
          .addImm(static_cast<uint32_t>(Offset) >> 16);
      BuildMI(*MI.getParent(), II, DL, TII->get(SRRArch::OR_I_LO), Reg)
          .addReg(Reg)
          .addImm(Offset & 0xffffU);
    } else {
      // Reg = mov(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(SRRArch::ADD_I_LO), Reg)
          .addImm(0)
          .addImm(Offset);
    }
    // Reg = FrameReg OP Reg
    if (MI.getOpcode() == SRRArch::ADD_I_LO) {
      BuildMI(*MI.getParent(), II, DL,
              HasNegOffset ? TII->get(SRRArch::SUB_R) : TII->get(SRRArch::ADD_R),
              MI.getOperand(0).getReg())
          .addReg(FrameReg)
          .addReg(Reg)
          .addImm(LPCC::ICC_T);
      MI.eraseFromParent();
      return true;
    }
    if (isSPLSOpcode(MI.getOpcode()) || isRMOpcode(MI.getOpcode())) {
      MI.setDesc(TII->get(getRRMOpcodeVariant(MI.getOpcode())));
      if (HasNegOffset) {
        // Change the ALU op (operand 3) from LPAC::ADD (the default) to
        // LPAC::SUB with the already negated offset.
        assert((MI.getOperand(3).getImm() == LPAC::ADD) &&
               "Unexpected ALU op in RRM instruction");
        MI.getOperand(3).setImm(LPAC::SUB);
      }
    } else
      llvm_unreachable("Unexpected opcode in frame index operation");

    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1)
        .ChangeToRegister(Reg, /*isDef=*/false, /*isImp=*/false,
                          /*isKill=*/true);
    return false;
  }

  // ALU arithmetic ops take unsigned immediates. If the offset is negative,
  // we replace the instruction with one that inverts the opcode and negates
  // the immediate.
  if ((Offset < 0) && isALUArithLoOpcode(MI.getOpcode())) {
    unsigned NewOpcode = getOppositeALULoOpcode(MI.getOpcode());
    // We know this is an ALU op, so we know the operands are as follows:
    // 0: destination register
    // 1: source register (frame register)
    // 2: immediate
    BuildMI(*MI.getParent(), II, DL, TII->get(NewOpcode),
            MI.getOperand(0).getReg())
        .addReg(FrameReg)
        .addImm(-Offset);
    MI.eraseFromParent();
    return true;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

bool SRRArchRegisterInfo::hasBasePointer(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  // When we need stack realignment and there are dynamic allocas, we can't
  // reference off of the stack pointer, so we reserve a base pointer.
  if (hasStackRealignment(MF) && MFI.hasVarSizedObjects())
    return true;

  return false;
}

unsigned SRRArchRegisterInfo::getRARegister() const { return SRRArch::RCA; }

Register
SRRArchRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return SRRArch::FP;
}

Register SRRArchRegisterInfo::getBaseRegister() const { return SRRArch::R14; }

const uint32_t *
SRRArchRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                        CallingConv::ID /*CC*/) const {
  return CSR_RegMask;
}
