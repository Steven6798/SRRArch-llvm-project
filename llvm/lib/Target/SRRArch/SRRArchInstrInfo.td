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
defvar ADD_OPC = 0x01;
defvar SUB_OPC = 0x02;
defvar MUL_OPC = 0x03;
defvar SDIV_OPC = 0x04;
defvar UDIV_OPC = 0x05;
defvar AND_OPC = 0x06;
defvar OR_OPC = 0x07;
defvar XOR_OPC = 0x08;
defvar SHL_OPC = 0x09;
defvar SRA_OPC = 0x0a;
defvar SRL_OPC = 0x0b;
defvar CMPEQ_OPC = 0x0c;
defvar CMPNE_OPC = 0x0d;
defvar CMPLT_OPC = 0x0e;
defvar CMPGT_OPC = 0x0f;
defvar STORE_OPC = 0x10;
defvar LOAD_OPC = 0x11;
defvar RETURN_OPC = 0x12;
defvar GENINT_OPC = 0x13;
defvar MOV_OPC = 0x14;

// -------------------------------------------------------------------------- //
// Instructions
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// Arithmetic
// -------------------------------------------------------------------------- //

def ADD : InstrArith<ADD_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "ADD $Rd, $Rs1, $Rs2", []>;

def SUB : InstrArith<SUB_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SUB $Rd, $Rs1, $Rs2", []>;

def MUL : InstrArith<MUL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "MUL $Rd, $Rs1, $Rs2", []>;

def SDIV : InstrArith<SDIV_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SDIV $Rd, $Rs1, $Rs2", []>;

def UDIV : InstrArith<UDIV_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "UDIV $Rd, $Rs1, $Rs2", []>;

// -------------------------------------------------------------------------- //
// Bitwise
// -------------------------------------------------------------------------- //

def AND : InstrArith<AND_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "AND $Rd, $Rs1, $Rs2", []>;

def OR : InstrArith<OR_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "OR $Rd, $Rs1, $Rs2", []>;

def XOR : InstrArith<XOR_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "XOR $Rd, $Rs1, $Rs2", []>;

// -------------------------------------------------------------------------- //
// Shift
// -------------------------------------------------------------------------- //

def SHL : InstrArith<SHL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SHL $Rd, $Rs1, $Rs2", []>;

def SRA : InstrArith<SRA_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SRA $Rd, $Rs1, $Rs2", []>;

def SRL : InstrArith<SRL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SRL $Rd, $Rs1, $Rs2", []>;

// -------------------------------------------------------------------------- //
// Comparison
// -------------------------------------------------------------------------- //

def CMPEQ : InstrArith<CMPEQ_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "CMPEQ $Rd, $Rs1, $Rs2", []>;

def CMPNE : InstrArith<CMPNE_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "CMPNE $Rd, $Rs1, $Rs2", []>;

def CMPLT : InstrArith<CMPLT_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "CMPLT $Rd, $Rs1, $Rs2", []>;

def CMPGT : InstrArith<CMPGT_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "CMPGT $Rd, $Rs1, $Rs2", []>;

// -------------------------------------------------------------------------- //
// Memory Access
// -------------------------------------------------------------------------- //

def STORE : InstrMemory<STORE_OPC, (outs), (ins GPR:$Rd, GPR:$Rs1),
            "STORE $Rd, $Rs1", []>;

def LOAD : InstrMemory<LOAD_OPC, (outs GPR:$Rd), (ins GPR:$Rs1),
           "LOAD $Rd, $Rs1", []>;


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

def MOV : InstSRRArch<(outs GPR:$Rd), (ins GPR:$Rs1), "MOV $Rd, $Rs1", []> {
  bits<5> Rd;
  bits<5> Rs1;

  let Inst{7 - 0}   = MOV_OPC; // Opcode
  let Inst{12 - 8}  = Rd;      // Destination Register
  let Inst{17 - 13} = Rs1;     // Source Register 1
  let Inst{63 - 17} = 0;       // Reserved
}

// -------------------------------------------------------------------------- //
// Branch
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