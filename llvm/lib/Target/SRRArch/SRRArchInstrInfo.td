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

defvar RETURN_OPC = 0x01;

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
