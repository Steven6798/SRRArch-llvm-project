//===-- SRRArchTargetTransformInfo.h - SRRArch specific TTI ---------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file a TargetTransformInfoImplBase conforming object specific to the
// SRRArch target machine. It uses the target's detailed information to
// provide more precise answers to certain TTI queries, while letting the
// target independent and default TTI implementations handle the rest.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_SRRARCHTARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_SRRARCH_SRRARCHTARGETTRANSFORMINFO_H

#include "SRRArch.h"
#include "SRRArchSubtarget.h"
#include "SRRArchTargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/BasicTTIImpl.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
class SRRArchTTIImpl final : public BasicTTIImplBase<SRRArchTTIImpl> {
  typedef BasicTTIImplBase<SRRArchTTIImpl> BaseT;
  typedef TargetTransformInfo TTI;
  friend BaseT;

  const SRRArchSubtarget *ST;
  const SRRArchTargetLowering *TLI;

  const SRRArchSubtarget *getST() const { return ST; }
  const SRRArchTargetLowering *getTLI() const { return TLI; }

public:
  explicit SRRArchTTIImpl(const SRRArchTargetMachine *TM, const Function &F)
      : BaseT(TM, F.getDataLayout()), ST(TM->getSubtargetImpl(F)),
        TLI(ST->getTargetLowering()) {}
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_SRRARCH_SRRARCHTARGETTRANSFORMINFO_H
