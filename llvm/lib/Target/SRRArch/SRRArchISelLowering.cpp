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
#include "llvm/CodeGen/MachineFrameInfo.h"

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

  setOperationAction(ISD::FrameIndex, MVT::i64, Custom);
  setOperationAction(ISD::GlobalAddress, MVT::i64, Custom);

  // Function alignments
  setMinFunctionAlignment(Align(8));
  setPrefFunctionAlignment(Align(8));

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}

SDValue SRRArchTargetLowering::LowerOperation(SDValue Op,
                                              SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::FrameIndex:
    return LowerFrameIndex(Op, DAG);
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  default:
    llvm_unreachable("unimplemented operand");
  }
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
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  // bool &IsTailCall = CLI.IsTailCall;
  CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg;

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState ArgCCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), ArgLocs,
                    *DAG.getContext());
  // GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee);
  // MachineFrameInfo &MFI = DAG.getMachineFunction().getFrameInfo();

  if (IsVarArg) {
    llvm_unreachable("function call variable arguments not supported yet");
  } else {
    ArgCCInfo.AnalyzeCallOperands(Outs, CC_SRRArch);
  }

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = ArgCCInfo.getStackSize();

  // Create local copies for byval args.
  SmallVector<SDValue, 8> ByValArgs;
  for (unsigned I = 0, E = Outs.size(); I != E; ++I) {
    llvm_unreachable("function call byval arguments not supported yet");
  }

  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, DL);

  SmallVector<std::pair<unsigned, SDValue>, 4> RegsToPass;
  SmallVector<SDValue, 12> MemOpChains;
  // SDValue StackPtr;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned I = 0, J = 0, E = ArgLocs.size(); I != E; ++I) {
    llvm_unreachable("function call arguments not supported yet");
    CCValAssign &VA = ArgLocs[I];
    SDValue Arg = OutVals[I];
    ISD::ArgFlagsTy Flags = Outs[I].Flags;

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
    case CCValAssign::Full:
      break;
    case CCValAssign::SExt:
      Arg = DAG.getNode(ISD::SIGN_EXTEND, DL, VA.getLocVT(), Arg);
      break;
    case CCValAssign::ZExt:
      Arg = DAG.getNode(ISD::ZERO_EXTEND, DL, VA.getLocVT(), Arg);
      break;
    case CCValAssign::AExt:
      Arg = DAG.getNode(ISD::ANY_EXTEND, DL, VA.getLocVT(), Arg);
      break;
    default:
      llvm_unreachable("Unknown loc info!");
    }

    // Use local copy if it is a byval arg.
    if (Flags.isByVal())
      Arg = ByValArgs[J++];

    // Arguments that can be passed on register must be kept at RegsToPass
    // vector
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      llvm_unreachable("Arguments passed on the stack not supported yet");
    }
  }

  SDValue InGlue;

  // Build a sequence of copy-to-reg nodes chained together with token chain and
  // flag operands which copy the outgoing args into registers.  The InGlue in
  // necessary since all emitted instructions must be stuck together.
  for (const auto &[Reg, N] : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, DL, Reg, N, InGlue);
    InGlue = Chain.getValue(1);
  }

  // Returns a chain & a flag for retval copy to use.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add a register mask operand representing the call-preserved registers.
  const uint32_t *Mask =
      TRI->getCallPreservedMask(DAG.getMachineFunction(), CallConv);
  assert(Mask && "Missing call preserved mask for calling convention");
  Ops.push_back(DAG.getRegisterMask(Mask));

  // Add argument registers to the end of the list so that they are
  // known live into the call.
  for (const auto &[Reg, N] : RegsToPass)
    Ops.push_back(DAG.getRegister(Reg, N.getValueType()));

  // Set pc + 24(8*3) to the RA register. There are 3 instruction that we have
  // to jump to get the address after the call: ADD, MOV and CALL
  SDValue PC = DAG.getRegister(SRRArch::R0, getPointerTy(DAG.getDataLayout()));
  SDValue Offset = DAG.getConstant(24, DL, MVT::i64);
  SDValue PCAddr = DAG.getNode(ISD::ADD, DL, MVT::i64, PC, Offset);
  // InGlue = Chain.getValue(1);

  Chain = DAG.getCopyToReg(Chain, DL, SRRArch::R4, PCAddr, InGlue);
  InGlue = Chain.getValue(1);

  if (InGlue.getNode())
    Ops.push_back(InGlue);

  Chain = DAG.getNode(SRRArchISD::CALL, DL, NodeTys,
                      ArrayRef<SDValue>(&Ops[0], Ops.size()));
  InGlue = Chain.getValue(1);

  // Create the CALLSEQ_END node.
  Chain = DAG.getCALLSEQ_END(Chain, NumBytes, 0, InGlue, DL);
  InGlue = Chain.getValue(1);

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState RetCCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                    *DAG.getContext());

  RetCCInfo.AnalyzeCallResult(Ins, RetCC_SRRArch);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned I = 0; I != RVLocs.size(); ++I) {
    llvm_unreachable("Function call returns not supported yet");
  }

  return Chain;
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

  SDValue Glue;
  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Glue);

    // Guarantee that all emitted copies are stuck together with flags.
    Glue = Chain.getValue(1);
  }

  return DAG.getNode(SRRArchISD::RET_GLUE, DL, MVT::Other, Chain);
}

//===----------------------------------------------------------------------===//
//                      Custom Lowerings
//===----------------------------------------------------------------------===//

SDValue SRRArchTargetLowering::LowerFrameIndex(SDValue Op,
                                               SelectionDAG &DAG) const {
  int FI = cast<FrameIndexSDNode>(Op)->getIndex();
  return DAG.getTargetFrameIndex(FI, Op.getValueType());
}

SDValue SRRArchTargetLowering::LowerGlobalAddress(SDValue Op,
                                                  SelectionDAG &DAG) const {
  SDLoc DL(Op);
  MVT VT = getPointerTy(DAG.getDataLayout());
  const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
  int64_t Offset = cast<GlobalAddressSDNode>(Op)->getOffset();

  // Create the TargetGlobalAddress node, folding in the constant offset.
  SDValue TGA = DAG.getTargetGlobalAddress(GV, DL, MVT::i64, Offset);

  // Insert a GENINT. For now this assumess that the absolute address fits in
  // 32-bits. If this is not the case then the linker will throw an error.
  return DAG.getNode(SRRArchISD::GENINT, DL, VT, TGA);
}