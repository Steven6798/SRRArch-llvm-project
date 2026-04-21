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

  setOperationAction(ISD::GlobalAddress, MVT::i64, Custom);

  setOperationAction(ISD::BR_CC, MVT::i64, Expand);

  setCondCodeAction(ISD::SETLE, MVT::i64, Expand);
  setCondCodeAction(ISD::SETGE, MVT::i64, Expand);
  setCondCodeAction(ISD::SETULE, MVT::i64, Expand);
  setCondCodeAction(ISD::SETUGE, MVT::i64, Expand);

  setOperationAction(ISD::SREM, MVT::i64, Expand);
  setOperationAction(ISD::UREM, MVT::i64, Expand);

  setOperationAction(ISD::SDIVREM, MVT::i64, Expand);
  setOperationAction(ISD::UDIVREM, MVT::i64, Expand);

  setOperationAction(ISD::SELECT_CC, MVT::i64, Expand);

  setOperationAction(ISD::MULHS, MVT::i64, Expand);
  setOperationAction(ISD::MULHU, MVT::i64, Expand);
  setOperationAction(ISD::SMUL_LOHI, MVT::i64, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i64, Expand);

  for (MVT VT : MVT::integer_valuetypes()) {
    setOperationAction(ISD::SIGN_EXTEND_INREG, VT, Expand);
  }

  for (MVT VT : MVT::integer_valuetypes()) {
    setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::EXTLOAD, VT, MVT::i1, Promote);
  }

  // Always lower memset, memcpy, and memmove intrinsics to load/store
  // instructions, rather then generating calls to memset, mempcy or memmove.
  // This is until we can compile libraries
  MaxStoresPerMemset = MaxStoresPerMemsetOptSize = ~0U;
  MaxStoresPerMemcpy = MaxStoresPerMemcpyOptSize = ~0U;
  MaxStoresPerMemmove = MaxStoresPerMemmoveOptSize = ~0U;

  // Function alignments
  setMinFunctionAlignment(Align(8));
  setPrefFunctionAlignment(Align(8));

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}

SDValue SRRArchTargetLowering::LowerOperation(SDValue Op,
                                              SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
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

static bool CC_SRRArch_VarArg(unsigned ValNo, MVT ValVT, MVT LocVT,
                              CCValAssign::LocInfo LocInfo,
                              ISD::ArgFlagsTy ArgFlags, Type *OrigTy,
                              CCState &State) {
  // Handle fixed arguments with default CC.
  if (!ArgFlags.isVarArg())
    return CC_SRRArch(ValNo, ValVT, LocVT, LocInfo, ArgFlags, OrigTy, State);

  // Promote i8/i16/i32 args to i64
  if (LocVT == MVT::i8 || LocVT == MVT::i16 || LocVT == MVT::i32) {
    LocVT = MVT::i64;
    if (ArgFlags.isSExt())
      LocInfo = CCValAssign::SExt;
    else if (ArgFlags.isZExt())
      LocInfo = CCValAssign::ZExt;
    else
      LocInfo = CCValAssign::AExt;
  }

  // VarArgs get passed on stack
  unsigned Offset = State.AllocateStack(8, Align(8));
  State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset, LocVT, LocInfo));
  return false;
}

SDValue SRRArchTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();
  // SRRArchMachineFunctionInfo *SMFI =
  // MF.getInfo<SRRArchMachineFunctionInfo>();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_SRRArch);

  for (const CCValAssign &VA : ArgLocs) {
    if (VA.isRegLoc()) {
      // Arguments passed in registers
      EVT RegVT = VA.getLocVT();
      switch (RegVT.getSimpleVT().SimpleTy) {
      case MVT::i64: {
        Register VReg = RegInfo.createVirtualRegister(&SRRArch::GPRRegClass);
        RegInfo.addLiveIn(VA.getLocReg(), VReg);
        SDValue ArgValue = DAG.getCopyFromReg(Chain, DL, VReg, RegVT);

        // If this is an 8/16/32-bit value, it is really passed promoted to 64
        // bits. Insert an assert[sz]ext to capture this, then truncate to the
        // right size.
        if (VA.getLocInfo() == CCValAssign::SExt)
          ArgValue = DAG.getNode(ISD::AssertSext, DL, RegVT, ArgValue,
                                 DAG.getValueType(VA.getValVT()));
        else if (VA.getLocInfo() == CCValAssign::ZExt)
          ArgValue = DAG.getNode(ISD::AssertZext, DL, RegVT, ArgValue,
                                 DAG.getValueType(VA.getValVT()));

        if (VA.getLocInfo() != CCValAssign::Full)
          ArgValue = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), ArgValue);

        InVals.push_back(ArgValue);
        break;
      }
      default:
        LLVM_DEBUG(dbgs() << "LowerFormalArguments Unhandled argument type: "
                          << RegVT << "\n");
        llvm_unreachable("unhandled argument type");
      }
    } else {
      // Only arguments passed on the stack should make it here.
      assert(VA.isMemLoc());
      // Load the argument to a virtual register
      unsigned ObjSize = VA.getLocVT().getSizeInBits() / 8;
      // Check that the argument fits in stack slot
      if (ObjSize > 8) {
        errs() << "LowerFormalArguments Unhandled argument type: "
               << VA.getLocVT() << "\n";
      }
      // Create the frame index object for this incoming parameter...
      int FI = MFI.CreateFixedObject(ObjSize, VA.getLocMemOffset(), true);

      // Create the SelectionDAG nodes corresponding to a load
      // from this parameter
      SDValue FIN = DAG.getFrameIndex(FI, MVT::i64);
      InVals.push_back(DAG.getLoad(
          VA.getLocVT(), DL, Chain, FIN,
          MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI)));
    }
  }

  if (MF.getFunction().hasStructRetAttr()) {
    llvm_unreachable("StructRet argument type not supported yet");
  }

  if (IsVarArg) {
    llvm_unreachable("Variable argument not supported yet");
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
  // MachineFrameInfo &MFI = DAG.getMachineFunction().getFrameInfo();

  if (IsVarArg) {
    ArgCCInfo.AnalyzeCallOperands(Outs, CC_SRRArch_VarArg);
  } else {
    ArgCCInfo.AnalyzeCallOperands(Outs, CC_SRRArch);
  }

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = ArgCCInfo.getStackSize();

  // Create local copies for byval args.
  SmallVector<SDValue, 8> ByValArgs;
  for (unsigned I = 0, E = Outs.size(); I != E; ++I) {
    ISD::ArgFlagsTy Flags = Outs[I].Flags;
    if (!Flags.isByVal())
      continue;

    llvm_unreachable("function call byval arguments not supported yet");
  }

  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, DL);

  SmallVector<std::pair<unsigned, SDValue>, 4> RegsToPass;
  SmallVector<SDValue, 12> MemOpChains;
  SDValue StackPtr;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned I = 0, J = 0, E = ArgLocs.size(); I != E; ++I) {
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
      assert(VA.isMemLoc());

      if (StackPtr.getNode() == nullptr)
        StackPtr = DAG.getCopyFromReg(Chain, DL, SRRArch::R1,
                                      getPointerTy(DAG.getDataLayout()));

      SDValue PtrOff =
          DAG.getNode(ISD::ADD, DL, getPointerTy(DAG.getDataLayout()), StackPtr,
                      DAG.getIntPtrConstant(VA.getLocMemOffset(), DL));

      MemOpChains.push_back(
          DAG.getStore(Chain, DL, Arg, PtrOff, MachinePointerInfo()));
    }
  }

  // Transform all store nodes into one single node because all store nodes are
  // independent of each other.
  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other,
                        ArrayRef<SDValue>(&MemOpChains[0], MemOpChains.size()));

  SDValue InGlue;

  // Build a sequence of copy-to-reg nodes chained together with token chain and
  // flag operands which copy the outgoing args into registers.  The InGlue in
  // necessary since all emitted instructions must be stuck together.
  for (const auto &[Reg, N] : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, DL, Reg, N, InGlue);
    InGlue = Chain.getValue(1);
  }

  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee)) {
    Callee = DAG.getTargetGlobalAddress(G->getGlobal(), DL,
                                        getPointerTy(DAG.getDataLayout()), 0);
  } else if (ExternalSymbolSDNode *E = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Callee = DAG.getTargetExternalSymbol(E->getSymbol(),
                                         getPointerTy(DAG.getDataLayout()));
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
    Chain = DAG.getCopyFromReg(Chain, DL, RVLocs[I].getLocReg(),
                               RVLocs[I].getValVT(), InGlue)
                .getValue(1);
    InGlue = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
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