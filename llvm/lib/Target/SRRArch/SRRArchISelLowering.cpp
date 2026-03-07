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
  // Set up the register classes.
  addRegisterClass(MVT::i64, &SRRArch::GPRRegClass);

  // Compute derived properties from the register classes
  TRI = STI.getRegisterInfo();
  computeRegisterProperties(TRI);

  setStackPointerRegisterToSaveRestore(SRRArch::R1);

  // Function alignments
  setMinFunctionAlignment(Align(8));
  setPrefFunctionAlignment(Align(8));

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
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
  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_SRRArch);

  for (const CCValAssign &VA : ArgLocs) {
    llvm_unreachable("Arguments not supported yet");
  }

  return Chain;
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
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, Context);

  return CCInfo.CheckReturn(Outs, RetCC_SRRArch);
  return false;
}

SDValue
SRRArchTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                   bool IsVarArg,
                                   const SmallVectorImpl<ISD::OutputArg> &Outs,
                                   const SmallVectorImpl<SDValue> &OutVals,
                                   const SDLoc &DL, SelectionDAG &DAG) const {
  // CCValAssign - represent the assignment of the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  // Analize return values.
  CCInfo.AnalyzeReturn(Outs, RetCC_SRRArch);

  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    llvm_unreachable("Return values not supported yet");
  }

  unsigned Opc = SRRArchISD::RET_GLUE;
  return DAG.getNode(Opc, DL, MVT::Other,
                     ArrayRef<SDValue>(&RetOps[0], RetOps.size()));
}

//===----------------------------------------------------------------------===//
//                      Custom Lowerings
//===----------------------------------------------------------------------===//
