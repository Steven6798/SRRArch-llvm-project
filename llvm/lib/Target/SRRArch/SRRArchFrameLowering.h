//===-- SRRArchFrameLowering.h - Define frame lowering for SRRArch -*-C++-*===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class implements SRRArch-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_SRRARCHFRAMELOWERING_H
#define LLVM_LIB_TARGET_SRRARCH_SRRARCHFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class BitVector;
class SRRArchSubtarget;

class SRRArchFrameLowering : public TargetFrameLowering {
protected:
  const SRRArchSubtarget &STI;

public:
  explicit SRRArchFrameLowering(const SRRArchSubtarget &Subtarget)
      : TargetFrameLowering(StackGrowsDown,
                            /*StackAlignment=*/Align(8),
                            /*LocalAreaOffset=*/0),
        STI(Subtarget) {}

  // emitProlog/emitEpilog - These methods insert prolog and epilog code into
  // the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;

protected:
  bool hasFPImpl(const MachineFunction & /*MF*/) const override { return true; }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_SRRARCH_SRRARCHFRAMELOWERING_H
