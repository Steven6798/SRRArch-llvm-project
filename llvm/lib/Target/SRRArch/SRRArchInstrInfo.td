//===-- SRRArchInstrInfo.td - Target Description for SRRArch Target -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the SRRArch instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "SRRArchInstrFormats.td"

// -------------------------------------------------------------------------- //
// Instruction Operands and Patterns
// -------------------------------------------------------------------------- //

def SDT_SRRArchRetGlue  : SDTypeProfile<0,  1, [SDTCisVT<0, i64>]>;

def RetGlue          : SDNode<"SRRArchISD::RET_GLUE", SDT_SRRArchRetGlue,
                              [SDNPHasChain, SDNPOptInGlue]>;

// -------------------------------------------------------------------------- //
// Opcodes
// -------------------------------------------------------------------------- //

defvar NOP_OPC = 0x00;
defvar RETURN_OPC = 0x01;
defvar GENINT_OPC = 0x02;
defvar SHL_OPC = 0x03;
defvar OR_OPC = 0x04;
defvar MOV_OPC = 0x04;

// -------------------------------------------------------------------------- //
// Instructions
// -------------------------------------------------------------------------- //

let isReturn = 1, isTerminator = 1, isBarrier = 1 in {
  def RETURN : InstSRRArch<(outs), (ins GPR:$Rv), "RETURN $Rv",
    [(RetGlue GPR:$Rv)]> {
    bits<5> Rv;

    let Inst{7 - 0}   = RETURN_OPC; // Opcode
    let Inst{12 - 8}  = Rv;         // Return value reg
    let Inst{63 - 13} = 0;          // Reserved
  }
}

def NOP : InstSRRArch<(outs), (ins), "NOP", []> {
  let Inst{7 - 0} = NOP_OPC; // Opcode
}

def GENINT : InstSRRArch<(outs GPR:$Rd), (ins i32imm:$Imm), "GENINT $Rd, $Imm", []> {
  bits<5> Rd;
  bits<32> Imm;

  let Inst{7 - 0}   = GENINT_OPC; // Opcode
  let Inst{12 - 8}  = Rd;         // Destination register
  let Inst{44 - 13} = Imm;        // Immediate
  let Inst{63 - 45} = 0;          // Reserved
}

def SHL : InstrArith<SHL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SHL $Rd, $Rs1, $Rs2", []>;

def OR : InstrArith<OR_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "OR $Rd, $Rs1, $Rs2", []>;

def MOV : InstSRRArch<(outs GPR:$Rd), (ins GPR:$Rs1), "MOV $Rd, $Rs1", []> {
  bits<5> Rd;
  bits<5> Rs1;

  let Inst{7 - 0}   = MOV_OPC;   // Opcode
  let Inst{12 - 8}  = Rd;    // Destination Register
  let Inst{17 - 13} = Rs1;   // Source Register 1
  let Inst{63 - 17} = 0;     // Reserved
}