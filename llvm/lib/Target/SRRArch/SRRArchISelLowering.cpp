//===-- SRRArchISelLowering.cpp - SRRArch DAG Lowering Implementation
//---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SRRArchTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "SRRArchISelLowering.h"
#include "MCTargetDesc/SRRArchBaseInfo.h"
#include "SRRArchMachineFunctionInfo.h"
#include "SRRArchSubtarget.h"
#include "SRRArchTargetObjectFile.h"
#include "llvm/CodeGen/CallingConvLower.h"

#define DEBUG_TYPE "srrarch-lower"

using namespace llvm;

SRRArchTargetLowering::SRRArchTargetLowering(const TargetMachine &TM,
                                             const SRRArchSubtarget &STI)
    : TargetLowering(TM, STI) {
  llvm_unreachable("SRRArchTargetLowering not implemented yet");
}

SDValue SRRArchTargetLowering::LowerOperation(SDValue Op,
                                              SelectionDAG &DAG) const {
  llvm_unreachable("LowerOperation not implemented yet");
}

//===----------------------------------------------------------------------===//
//                       SRRArch Inline Assembly Support
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//                      Calling Convention Implementation
//===----------------------------------------------------------------------===//

#include "SRRArchGenCallingConv.inc"

SDValue SRRArchTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  llvm_unreachable("LowerFormalArguments not implemented yet");
}

SDValue
SRRArchTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                 SmallVectorImpl<SDValue> &InVals) const {
  llvm_unreachable("LowerCall not implemented yet");
}

bool SRRArchTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv, MachineFunction &MF, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs, LLVMContext &Context,
    const Type *RetTy) const {
  llvm_unreachable("CanLowerReturn not implemented yet");
  return false;
}

SDValue
SRRArchTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                   bool IsVarArg,
                                   const SmallVectorImpl<ISD::OutputArg> &Outs,
                                   const SmallVectorImpl<SDValue> &OutVals,
                                   const SDLoc &DL, SelectionDAG &DAG) const {
  llvm_unreachable("LowerReturn not implemented yet");
}

//===----------------------------------------------------------------------===//
//                      Custom Lowerings
//===----------------------------------------------------------------------===//
