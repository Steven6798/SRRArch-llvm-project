//===- SRRArchCallingConv.td - Calling Conventions SRRArch -*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the SRRArch architecture.
//
//===----------------------------------------------------------------------===//

// SRRArch C Calling convention.
def CC_SRRArch : CallingConv<[
  // Promote i8/i16 args to i64
  CCIfType<[i8, i16, i32], CCPromoteToType<i64>>,

  // Put argument in registers if not a vararg call.
  CCIfNotVarArg<CCIfType<[i64], CCAssignToReg<[R5, R6, R7, R8]>>>,

  // Otherwise they are assigned to the stack in 8-byte aligned units.
  CCAssignToStack<8, 8>
]>;

// SRRArch C return-value convention.
def RetCC_SRRArch : CallingConv<[
  // Put return value in register.
  CCIfType<[i64], CCAssignToReg<[R3]>>
]>;

def CSR: CalleeSavedRegs<(add (sequence "R%i", 21, 31))>;
