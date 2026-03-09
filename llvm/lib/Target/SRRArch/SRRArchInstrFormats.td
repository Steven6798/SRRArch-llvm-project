//===- SRRArchInstrFormats.td - SRRArch Instruction Format -*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class InstSRRArch<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<64> Inst;
  let Size = 8;

  let Namespace = "SRRArch";
  let DecoderNamespace = "SRRArch";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

class InstrArith<bits<8> opc,dag outs, dag ins, string asmstr,
                  list<dag> pattern>
    : InstSRRArch<outs, ins, asmstr, pattern> {
  bits<5> Rd;
  bits<5> Rs1;
  bits<5> Rs2;

  let Inst{7 - 0}   = opc;   // Opcode
  let Inst{12 - 8}  = Rd;    // Destination Register
  let Inst{17 - 13} = Rs1;   // Source Register 1
  let Inst{22 - 18} = Rs2;   // Source Register 2
  let Inst{63 - 23} = 0;     // Reserved
}