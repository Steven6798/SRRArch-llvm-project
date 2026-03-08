//===- SRRArchRegisterInfo.td - SRRArch Register defs ------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the SRRArch register file
//===----------------------------------------------------------------------===//

// Registers are identified with 5-bit ID numbers.
class SRRArchReg<bits<5> num, string n, list<Register> subregs = [],
               list<string> altNames = []> : Register<n, altNames> {
  field bits<5> Num;
  let Num = num;
  let Namespace = "SRRArch";
  let SubRegs = subregs;
}

// Integer registers
foreach i = 0-32 in {
  def R#i : SRRArchReg<i, "R"#i>, DwarfRegNum<[i]>;
}

// Register classes.
def GPR : RegisterClass<"SRRArch", [i64], 64,
   (add (sequence "R%i", 20, 31), // callee save
   (sequence "R%i", 8, 19),       // caller save
    R4, R5, R6, R7,               // registers for passing arguments
    R3,                           // return value
    R2,                           // frame pointer
    R1,                           // stack pointer
    R0                            // program counter
   )>;

