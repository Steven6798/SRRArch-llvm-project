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
                                 [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                                  SDNPVariadic]>;
def SRRArchCALLREG      : SDNode<"SRRArchISD::CALLREG", SDT_SRRArchCALL,
                                 [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                                  SDNPVariadic]>;
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
    return ((N->getZExtValue() & 0xFFFFFFFFUL) == N->getZExtValue());}], LO32> {
}

def Imm12AsmOperand : AsmOperandClass { let Name = "Imm12"; }
def imm12 : Operand<i64>, PatLeaf<(imm), [{
    return isInt<12>(N->getSExtValue()); }]> {
  let ParserMatchClass = Imm12AsmOperand;
}

def BrTargetAsmOperand : AsmOperandClass {
  let Name = "BrTarget";
}
def BrTarget   : Operand<OtherVT> {
  let ParserMatchClass = BrTargetAsmOperand;
  let EncoderMethod = "getBranchTargetOpValue";
  let DecoderMethod = "decodeBranch";
}

def ADDRri : ComplexPattern<i64, 2, "selectAddrRi", [frameindex], []>;

def MEMri : Operand<i64> {
  let DecoderMethod = "decodeRiMemoryValue";
  let EncoderMethod = "getRiMemoryOpValue";
  let MIOperandInfo = (ops GPR:$base, imm12:$offset);
  let PrintMethod   = "printMemRiOperand";
}

// -------------------------------------------------------------------------- //
// Opcodes
// -------------------------------------------------------------------------- //

defvar NOP_OPC     = 0x00;
defvar ADD_OPC     = 0x01;
defvar SUB_OPC     = 0x02;
defvar MUL_OPC     = 0x03;
defvar SDIV_OPC    = 0x04;
defvar UDIV_OPC    = 0x05;
defvar AND_OPC     = 0x06;
defvar OR_OPC      = 0x07;
defvar XOR_OPC     = 0x08;
defvar SHL_OPC     = 0x09;
defvar SRA_OPC     = 0x0a;
defvar SRL_OPC     = 0x0b;
defvar CMPEQ_OPC   = 0x0c;
defvar CMPNE_OPC   = 0x0d;
defvar CMPLT_OPC   = 0x0e;
defvar CMPGT_OPC   = 0x0f;
defvar CMPULT_OPC  = 0x10;
defvar CMPUGT_OPC  = 0x11;
defvar ADDI_OPC    = 0x12;
defvar SUBI_OPC    = 0x13;
defvar ANDI_OPC    = 0x14;
defvar ORI_OPC     = 0x15;
defvar XORI_OPC    = 0x16;
defvar SHLI_OPC    = 0x17;
defvar SRAI_OPC    = 0x18;
defvar SRLI_OPC    = 0x19;
defvar STOREB_OPC  = 0x1a;
defvar STOREH_OPC  = 0x1b;
defvar STOREW_OPC  = 0x1c;
defvar STORE_OPC   = 0x1d;
defvar LOADBZ_OPC  = 0x1e;
defvar LOADBS_OPC  = 0x1f;
defvar LOADHZ_OPC  = 0x20;
defvar LOADHS_OPC  = 0x21;
defvar LOADWZ_OPC  = 0x22;
defvar LOADWS_OPC  = 0x23;
defvar LOAD_OPC    = 0x24;
defvar RETURN_OPC  = 0x25;
defvar CALL_OPC    = 0x26;
defvar CALLREG_OPC = 0x27;
defvar BRCOND_OPC  = 0x28;
defvar BR_OPC      = 0x29;
defvar GENINT_OPC  = 0x2a;
defvar MOV_OPC     = 0x2b;
defvar SELECT_OPC  = 0x2c;

// -------------------------------------------------------------------------- //
// Instructions
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
// Arithmetic register-register
// -------------------------------------------------------------------------- //

multiclass ALURRBase<bits<8> Opc, string AsmStr, SDNode OpNode> {
  def "" : InstrALURR<Opc, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
            !strconcat(AsmStr, " $Rd, $Rs1, $Rs2"),
            [(set GPR:$Rd, (OpNode GPR:$Rs1, GPR:$Rs2))]>;
}

defm ADD  : ALURRBase<ADD_OPC, "ADD", add>;
defm SUB  : ALURRBase<SUB_OPC, "SUB", sub>;
defm MUL  : ALURRBase<MUL_OPC, "MUL", mul>;
defm SDIV : ALURRBase<SDIV_OPC, "SDIV", sdiv>;
defm UDIV : ALURRBase<UDIV_OPC, "UDIV", udiv>;

// -------------------------------------------------------------------------- //
// Arithmetic register-immediate
// -------------------------------------------------------------------------- //

multiclass ALURIBase<bits<8> Opc, string AsmStr, SDNode OpNode> {
  def "" : InstrALURI<Opc, (outs GPR:$Rd), (ins GPR:$Rs1, imm12:$Imm),
            !strconcat(AsmStr, " $Rd, $Rs1, $Imm"),
            [(set GPR:$Rd, (OpNode GPR:$Rs1, imm12:$Imm))]>;
}

defm ADDI : ALURIBase<ADDI_OPC, "ADDI", add>;
defm SUBI : ALURIBase<SUBI_OPC, "SUBI", sub>;

// -------------------------------------------------------------------------- //
// Bitwise register-register
// -------------------------------------------------------------------------- //

defm AND : ALURRBase<AND_OPC, "AND", and>;
defm OR  : ALURRBase<OR_OPC, "OR", or>;
defm XOR : ALURRBase<XOR_OPC, "XOR", xor>;

// -------------------------------------------------------------------------- //
// Bitwise register-immediate
// -------------------------------------------------------------------------- //

defm ANDI : ALURIBase<ANDI_OPC, "ANDI", and>;
defm ORI  : ALURIBase<ORI_OPC, "ORI", or>;
defm XORI : ALURIBase<XORI_OPC, "XORI", xor>;

// -------------------------------------------------------------------------- //
// Shift register-register
// -------------------------------------------------------------------------- //

defm SHL : ALURRBase<SHL_OPC, "SHL", shl>;
defm SRA : ALURRBase<SRA_OPC, "SRA", sra>;
defm SRL : ALURRBase<SRL_OPC, "SRL", srl>;

// -------------------------------------------------------------------------- //
// Shift register-immediate
// -------------------------------------------------------------------------- //

defm SHLI : ALURIBase<SHLI_OPC, "SHLI", shl>;
defm SRAI : ALURIBase<SRAI_OPC, "SRAI", sra>;
defm SRLI : ALURIBase<SRLI_OPC, "SRLI", srl>;

// -------------------------------------------------------------------------- //
// Comparison
// -------------------------------------------------------------------------- //

multiclass CMPBase<bits<8> Opc, string AsmStr, PatFrag OpNode> {
  def "" : InstrALURR<Opc, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2),
            !strconcat(AsmStr, " $Rd, $Rs1, $Rs2"),
            [(set GPR:$Rd, (OpNode GPR:$Rs1, GPR:$Rs2))]>;
}

defm CMPEQ  : CMPBase<CMPEQ_OPC, "CMPEQ", seteq>;
defm CMPNE  : CMPBase<CMPNE_OPC, "CMPNE", setne>;
defm CMPLT  : CMPBase<CMPLT_OPC, "CMPLT", setlt>;
defm CMPGT  : CMPBase<CMPGT_OPC, "CMPGT", setgt>;
defm CMPULT : CMPBase<CMPULT_OPC, "CMPULT", setult>;
defm CMPUGT : CMPBase<CMPUGT_OPC, "CMPUGT", setugt>;

// -------------------------------------------------------------------------- //
// Memory Access
// -------------------------------------------------------------------------- //

multiclass LoadBase<bits<8> Opc, string AsmStr, PatFrag OpNode> {
  def "" : InstrLD<Opc, (outs GPR:$Rd), (ins MEMri:$Src),
                   !strconcat(AsmStr, " $Rd, $Src"),
                   [(set GPR:$Rd, (OpNode ADDRri:$Src))]>;
}

defm LOADBZ : LoadBase<LOADBZ_OPC, "LOADBZ", zextloadi8>;
defm LOADBS : LoadBase<LOADBS_OPC, "LOADBS", sextloadi8>;
defm LOADHZ : LoadBase<LOADHZ_OPC, "LOADHZ", zextloadi16>;
defm LOADHS : LoadBase<LOADHS_OPC, "LOADHS", sextloadi16>;
defm LOADWZ : LoadBase<LOADWZ_OPC, "LOADWZ", zextloadi32>;
defm LOADWS : LoadBase<LOADWS_OPC, "LOADWS", sextloadi32>;
defm LOAD   : LoadBase<LOAD_OPC, "LOAD", load>;

// extload -> zextload
def : Pat<(extloadi8 ADDRri:$Rs1), (LOADBZ MEMri:$Rs1)>;
def : Pat<(extloadi16 ADDRri:$Rs1), (LOADHZ MEMri:$Rs1)>;
def : Pat<(extloadi32 ADDRri:$Rs1), (LOADWZ MEMri:$Rs1)>;

multiclass StoreBase<bits<8> Opc, string AsmStr, PatFrag OpNode> {
  def "" : InstrST<Opc, (outs), (ins GPR:$Rs1, MEMri:$Src),
                   !strconcat(AsmStr, " $Rs1, $Src"),
                   [(OpNode GPR:$Rs1, ADDRri:$Src)]>;
}

defm STOREB : StoreBase<STOREB_OPC, "STOREB", truncstorei8>;
defm STOREH : StoreBase<STOREH_OPC, "STOREH", truncstorei16>;
defm STOREW : StoreBase<STOREW_OPC, "STOREW", truncstorei32>;
defm STORE  : StoreBase<STORE_OPC, "STORE", store>;

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
  def CALL : InstSRRArch<(outs), (ins i64lo32z:$Addr), "CALL $Addr",
                         [(SRRArchCALL tglobaladdr:$Addr)]> {
    bits<32> Addr;

    let Inst{7 - 0}   = CALL_OPC; // Opcode
    let Inst{39 - 8}  = Addr;     // Address
    let Inst{63 - 40} = 0;        // Reserved
  }

  def CALLREG : InstSRRArch<(outs), (ins GPR:$Rs1), "CALLREG $Rs1",
                         [(SRRArchCALLREG GPR:$Rs1)]> {
    bits<5> Rs1;

    let Inst{7 - 0}   = CALLREG_OPC; // Opcode
    let Inst{12 - 8}  = Rs1;         // Source register
    let Inst{63 - 13} = 0;           // Reserved
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
  let isMoveReg = 1;
  bits<5> Rd;
  bits<5> Rs1;

  let Inst{7 - 0}   = MOV_OPC; // Opcode
  let Inst{12 - 8}  = Rd;      // Destination Register
  let Inst{17 - 13} = Rs1;     // Source Register 1
  let Inst{63 - 18} = 0;       // Reserved
}

def SELECT : InstSRRArch<(outs GPR:$Rd),
                         (ins GPR:$RC, GPR:$RT, GPR:$RF),
                         "SELECT $Rd, $RC, $RT, $RF",
                         [(set GPR:$Rd, (select GPR:$RC, GPR:$RT, GPR:$RF))]> {
  bits<5> Rd;
  bits<5> RC;
  bits<5> RT;
  bits<5> RF;

  let Inst{7 - 0}   = SELECT_OPC; // Opcode
  let Inst{12 - 8}  = Rd;         // Destination Register
  let Inst{17 - 13} = RC;         // Condition Register
  let Inst{22 - 18} = RT;         // True Source
  let Inst{27 - 23} = RF;         // False Source
  let Inst{63 - 28} = 0;          // Reserved
}