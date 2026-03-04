//===-- SRRArch.h - Top-level interface for SRRArch representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// SRRArch back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_SRRARCH_H
#define LLVM_LIB_TARGET_SRRARCH_SRRARCH_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class SRRArchTargetMachine;
class PassRegistry;

// createSRRArchISelDag - This pass converts a legalized DAG into a
// SRRArch-specific DAG, ready for instruction scheduling.
FunctionPass *createSRRArchISelDag(SRRArchTargetMachine &TM);

// createSRRArchDelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createSRRArchDelaySlotFillerPass(const SRRArchTargetMachine &TM);

// createSRRArchMemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createSRRArchMemAluCombinerPass();

// createSRRArchSetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createSRRArchSetflagAluCombinerPass();

void initializeSRRArchAsmPrinterPass(PassRegistry &);
void initializeSRRArchDAGToDAGISelLegacyPass(PassRegistry &);
void initializeSRRArchMemAluCombinerPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_SRRARCH_SRRARCH_H
