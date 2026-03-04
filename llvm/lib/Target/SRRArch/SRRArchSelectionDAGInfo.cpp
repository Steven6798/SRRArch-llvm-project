//===-- SRRArchSelectionDAGInfo.cpp - SRRArch SelectionDAG Info -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SRRArchSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "SRRArchSelectionDAGInfo.h"

#define GET_SDNODE_DESC
#include "SRRArchGenSDNodeInfo.inc"

#define DEBUG_TYPE "srrarch-selectiondag-info"

using namespace llvm;

SRRArchSelectionDAGInfo::SRRArchSelectionDAGInfo()
    : SelectionDAGGenTargetInfo(SRRArchGenSDNodeInfo) {}

SDValue SRRArchSelectionDAGInfo::EmitTargetCodeForMemcpy(
    SelectionDAG & /*DAG*/, const SDLoc & /*dl*/, SDValue /*Chain*/,
    SDValue /*Dst*/, SDValue /*Src*/, SDValue Size, Align /*Alignment*/,
    bool /*isVolatile*/, bool /*AlwaysInline*/,
    MachinePointerInfo /*DstPtrInfo*/,
    MachinePointerInfo /*SrcPtrInfo*/) const {
  ConstantSDNode *ConstantSize = dyn_cast<ConstantSDNode>(Size);
  if (!ConstantSize)
    return SDValue();

  return SDValue();
}
