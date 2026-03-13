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
class SRRArchReg<bits<5> enc, string n, list<Register> subregs = [],
               list<string> altNames = []> : Register<n, altNames> {
  let HWEncoding{4-0}  = enc;
  let HWEncoding{15-5} = 0;
  let Namespace = "SRRArch";
  let SubRegs = subregs;
}

// Integer registers
foreach i = 0-31 in {
  def R#i : SRRArchReg<i, "R"#i>, DwarfRegNum<[i]>;
}

// Register classes.
def GPR : RegisterClass<"SRRArch", [i64], 64,
   (add (sequence "R%i", 21, 31), // callee save
   (sequence "R%i", 9, 20),       // caller save
    R5, R6, R7, R8,               // registers for passing arguments
    R4,                           // return address
    R3,                           // return value
    R2,                           // frame pointer
    R1,                           // stack pointer
    R0                            // program counter
   )>;

