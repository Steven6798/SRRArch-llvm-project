//===-- SRRArchTargetInfo.cpp - SRRArch Target Implementation -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/SRRArchTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheSRRArchTarget() {
  static Target TheSRRArchTarget;
  return TheSRRArchTarget;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeSRRArchTargetInfo() {
  RegisterTarget<Triple::srrarch> X(getTheSRRArchTarget(), "srrarch", "SRRArch",
                                    "SRRArch");
}
