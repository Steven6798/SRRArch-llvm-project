//===-- SRRArchInstPrinter.cpp - Convert SRRArch MCInst to asm syntax -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an SRRArch MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "SRRArchInstPrinter.h"
#include "MCTargetDesc/SRRArchMCTargetDesc.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "SRRArchGenAsmWriter.inc"

void SRRArchInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  OS << StringRef(getRegisterName(Reg)).lower();
}

void SRRArchInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                   StringRef Annotation,
                                   const MCSubtargetInfo & /*STI*/,
                                   raw_ostream &OS) {
  printInstruction(MI, Address, OS);

  // Next always print the annotation.
  printAnnotation(OS, Annotation);
}

void SRRArchInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg())
    OS << getRegisterName(Op.getReg());
  else if (Op.isImm())
    OS << formatHex(Op.getImm());
  else {
    assert(Op.isExpr() && "Expected an expression");
    MAI.printExpr(OS, *Op.getExpr());
  }
}