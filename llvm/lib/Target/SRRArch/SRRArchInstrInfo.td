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

def SDT_SRRArchRetGlue      : SDTypeProfile<0,  0, []>;
def SDT_SRRArchGENINT       : SDTypeProfile<1,  1, [SDTCisVT<0, i64>]>;
def SDT_SRRArchCALL         : SDTypeProfile<0, -1, [SDTCisVT<0, i64>]>;
def SDT_SRRArchCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i64>,
                                              SDTCisVT<1, i64>]>;
def SDT_SRRArchCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i64>,
                                            SDTCisVT<1, i64>]>;

def SRRArchRetGlue      : SDNode<"SRRArchISD::RET_GLUE", SDT_SRRArchRetGlue,
                                 [SDNPHasChain, SDNPOptInGlue]>;
def SRRArchGENINT       : SDNode<"SRRArchISD::GENINT", SDT_SRRArchGENINT,
                                 []>;
def SRRArchCALL         : SDNode<"SRRArchISD::CALL", SDT_SRRArchCALL,
                                 [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def SRRArchCallSeqStart : SDNode<"ISD::CALLSEQ_START", SDT_SRRArchCallSeqStart,
                                 [SDNPHasChain, SDNPOutGlue]>;
def SRRArchCallSeqEnd   : SDNode<"ISD::CALLSEQ_END", SDT_SRRArchCallSeqEnd,
                                 [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
// Operand and SDNode transformation definitions.
//===----------------------------------------------------------------------===//

// Extract bits 0-31 (low-end) of an immediate value.
def LO32 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xffffffff,
                                   SDLoc(N), MVT::i64);
}]>;

def i64lo32z : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo32 predicate - true if the 64-bit immediate has only rightmost 32
    // bits set.
    return ((int64_t)(N->getZExtValue() & 0xFFFFFFFFUL) == N->getZExtValue());}], LO32> {
}

def BrTarget   : Operand<OtherVT> {
  let EncoderMethod = "getBranchTargetOpValue";
  let DecoderMethod = "decodeBranch";
}

// -------------------------------------------------------------------------- //
// Opcodes
// -------------------------------------------------------------------------- //

defvar NOP_OPC    = 0x00;
defvar ADD_OPC    = 0x01;
defvar SUB_OPC    = 0x02;
defvar MUL_OPC    = 0x03;
defvar SDIV_OPC   = 0x04;
defvar UDIV_OPC   = 0x05;
defvar AND_OPC    = 0x06;
defvar OR_OPC     = 0x07;
defvar XOR_OPC    = 0x08;
defvar SHL_OPC    = 0x09;
defvar SRA_OPC    = 0x0a;
defvar SRL_OPC    = 0x0b;
defvar CMPEQ_OPC  = 0x0c;
defvar CMPNE_OPC  = 0x0d;
defvar CMPLT_OPC  = 0x0e;
defvar CMPGT_OPC  = 0x0f;
defvar STORE_OPC  = 0x10;
defvar LOAD_OPC   = 0x11;
defvar RETURN_OPC = 0x12;
defvar GENINT_OPC = 0x13;
defvar MOV_OPC    = 0x14;
defvar CALL_OPC   = 0x15;
defvar BRCOND_OPC = 0x16;
defvar BR_OPC     = 0x17;

// -------------------------------------------------------------------------- //
// Instructions
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// Arithmetic
// -------------------------------------------------------------------------- //

def ADD : InstrArith<ADD_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "ADD $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (add (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def SUB : InstrArith<SUB_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SUB $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (sub (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def MUL : InstrArith<MUL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "MUL $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (mul (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def SDIV : InstrArith<SDIV_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
           "SDIV $Rd, $Rs1, $Rs2",
           [(set (i64 GPR:$Rd), (sdiv (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def UDIV : InstrArith<UDIV_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
           "UDIV $Rd, $Rs1, $Rs2",
           [(set (i64 GPR:$Rd), (udiv (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

// -------------------------------------------------------------------------- //
// Bitwise
// -------------------------------------------------------------------------- //

def AND : InstrArith<AND_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "AND $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (and (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def OR : InstrArith<OR_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
         "OR $Rd, $Rs1, $Rs2",
         [(set (i64 GPR:$Rd), (or (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def XOR : InstrArith<XOR_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "XOR $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (xor (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

// -------------------------------------------------------------------------- //
// Shift
// -------------------------------------------------------------------------- //

def SHL : InstrArith<SHL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SHL $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (shl (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def SRA : InstrArith<SRA_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SRA $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (sra (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def SRL : InstrArith<SRL_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
          "SRL $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (srl (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

// -------------------------------------------------------------------------- //
// Comparison
// -------------------------------------------------------------------------- //

def CMPEQ : InstrArith<CMPEQ_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
            "CMPEQ $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (seteq (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def CMPNE : InstrArith<CMPNE_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
            "CMPNE $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (setne (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def CMPLT : InstrArith<CMPLT_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
            "CMPLT $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (setlt (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

def CMPGT : InstrArith<CMPGT_OPC, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
            "CMPGT $Rd, $Rs1, $Rs2",
          [(set (i64 GPR:$Rd), (setgt (i64 GPR:$Rs1), (i64 GPR:$Rs2)))]>;

// -------------------------------------------------------------------------- //
// Memory Access
// -------------------------------------------------------------------------- //

def STORE : InstrMemory<STORE_OPC, (outs), (ins GPR:$Rd, GPR:$Rs1),
            "STORE $Rd, $Rs1",
            [(store (i64 GPR:$Rs1), (i64 tframeindex:$Rd))]>;

def LOAD : InstrMemory<LOAD_OPC, (outs GPR:$Rd), (ins GPR:$Rs1),
           "LOAD $Rd, $Rs1",
           [(set (i64 GPR:$Rd), (load (i64 tframeindex:$Rs1)))]>;

def : Pat<(load GPR:$Rs1), (LOAD GPR:$Rs1)>;


def NOP : InstSRRArch<(outs), (ins), "NOP", []> {
  let Inst{7 - 0} = NOP_OPC; // Opcode
}

def GENINT : InstSRRArch<(outs GPR:$Rd), (ins i64lo32z:$Imm), "GENINT $Rd, $Imm",
             [(set (i64 GPR:$Rd), (SRRArchGENINT (i64 tglobaladdr:$Imm)))]> {
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

let isReturn = 1, isTerminator = 1, isBarrier = 1, Uses = [R3], Uses = [R4] in {
  def RETURN : InstSRRArch<(outs), (ins), "RETURN", [(SRRArchRetGlue)]> {
    let Inst{7 - 0}   = RETURN_OPC; // Opcode
    let Inst{63 - 8}  = 0;          // Reserved
  }
}

let isCall = 1, Uses = [R1], Defs = [R4] in {
  def CALL : InstSRRArch<(outs), (ins GPR:$Rs1), "CALL $Rs1",
                         [(SRRArchCALL GPR:$Rs1)]> {
    bits<5> Rs1;

    let Inst{7 - 0}   = CALL_OPC;   // Opcode
    let Inst{12 - 8}  = Rs1;        // Source register
    let Inst{63 - 13} = 0;          // Reserved
  }
}

let isBranch = 1, isTerminator = 1 in {
  def BRCOND : InstSRRArch<(outs), (ins GPR:$Rs1, BrTarget:$Addr),
                           "BRCOND $Rs1, $Addr",
                           [(brcond GPR:$Rs1, bb:$Addr)]> {
    bits<5> Rs1;
    bits<32> Addr;

    let Inst{7 - 0}   = BRCOND_OPC; // Opcode
    let Inst{12 - 8}  = Rs1;        // Source register
    let Inst{44 - 13} = Addr;       // Address
    let Inst{63 - 45} = 0;          // Reserved
  }
}

let isBranch = 1, isBarrier = 1, isTerminator = 1 in {
  def BR : InstSRRArch<(outs), (ins BrTarget:$Addr),
                       "BR $Addr", [(br bb:$Addr)]> {
    bits<32> Addr;

    let Inst{7 - 0}   = BR_OPC;     // Opcode
    let Inst{39 - 8}  = Addr;       // Address
    let Inst{63 - 40} = 0;          // Reserved
  }
}

// ADJCALLSTACKDOWN/UP implicitly use/def SP because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
// Pessimistically assume ADJCALLSTACKDOWN / ADJCALLSTACKUP will become
// sub / add which can clobber SP.
let Defs = [R1], Uses = [R1], isPseudo = 1 in {
  def ADJCALLSTACKDOWN : InstSRRArch<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                                     "#ADJCALLSTACKDOWN $amt1 $amt2",
                                [(SRRArchCallSeqStart timm:$amt1, timm:$amt2)]>;
  def ADJCALLSTACKUP   : InstSRRArch<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                                     "#ADJCALLSTACKUP $amt1 $amt2",
                                [(SRRArchCallSeqEnd timm:$amt1, timm:$amt2)]>;
}