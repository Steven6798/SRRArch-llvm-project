//===- SRRArch.td - Describe the SRRArch Target Machine --------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "SRRArchSchedule.td"
include "SRRArchRegisterInfo.td"
include "SRRArchCallingConv.td"
include "SRRArchInstrInfo.td"

defm : RemapAllTargetPseudoPointerOperands<GPR>;

def SRRArchInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// SRRArch processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", SRRArchSchedModel, []>;

def SRRArchInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def SRRArch : Target {
  // Pull in Instruction Info:
  let InstructionSet = SRRArchInstrInfo;
  let AssemblyWriters = [SRRArchInstPrinter];
}
