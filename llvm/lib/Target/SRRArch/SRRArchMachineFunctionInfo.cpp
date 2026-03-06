//===-- SRRArchMachineFuctionInfo.cpp - SRRArch machine function info -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SRRArchMachineFunctionInfo.h"

using namespace llvm;

void SRRArchMachineFunctionInfo::anchor() {}

MachineFunctionInfo *SRRArchMachineFunctionInfo::clone(
    BumpPtrAllocator &Allocator, MachineFunction &DestMF,
    const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
    const {
  return DestMF.cloneInfo<SRRArchMachineFunctionInfo>(*this);
}
