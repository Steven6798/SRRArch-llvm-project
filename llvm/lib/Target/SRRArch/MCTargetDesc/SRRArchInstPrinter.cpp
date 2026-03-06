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
#include "llvm/MC/MCInst.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "SRRArchGenAsmWriter.inc"

void SRRArchInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
  llvm_unreachable("printRegName not implemented yet");
}

bool SRRArchInstPrinter::printInst(const MCInst *MI, raw_ostream &OS,
                                   StringRef Alias, unsigned OpNo0,
                                   unsigned OpNo1) {
  llvm_unreachable("printInst not implemented yet");
  return false;
}

bool SRRArchInstPrinter::printAlias(const MCInst *MI, raw_ostream &OS) {
  llvm_unreachable("printAlias not implemented yet");
}

void SRRArchInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                   StringRef Annotation,
                                   const MCSubtargetInfo & /*STI*/,
                                   raw_ostream &OS) {
  llvm_unreachable("printInst not implemented yet");
}

void SRRArchInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &OS) {
  llvm_unreachable("printOperand not implemented yet");
}

void SRRArchInstPrinter::printMemImmOperand(const MCInst *MI, unsigned OpNo,
                                            raw_ostream &OS) {
  llvm_unreachable("printMemImmOperand not implemented yet");
}

void SRRArchInstPrinter::printHi16ImmOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &OS) {
  llvm_unreachable("printHi16ImmOperand not implemented yet");
}

void SRRArchInstPrinter::printHi16AndImmOperand(const MCInst *MI, unsigned OpNo,
                                                raw_ostream &OS) {
  llvm_unreachable("printHi16AndImmOperand not implemented yet");
}

void SRRArchInstPrinter::printLo16AndImmOperand(const MCInst *MI, unsigned OpNo,
                                                raw_ostream &OS) {
  llvm_unreachable("printLo16AndImmOperand not implemented yet");
}

void SRRArchInstPrinter::printMemRiOperand(const MCInst *MI, int OpNo,
                                           raw_ostream &OS) {
  llvm_unreachable("printMemRiOperand not implemented yet");
}

void SRRArchInstPrinter::printMemRrOperand(const MCInst *MI, int OpNo,
                                           raw_ostream &OS) {
  llvm_unreachable("printMemRrOperand not implemented yet");
}

void SRRArchInstPrinter::printMemSplsOperand(const MCInst *MI, int OpNo,
                                             raw_ostream &OS) {
  llvm_unreachable("printMemSplsOperand not implemented yet");
}

void SRRArchInstPrinter::printCCOperand(const MCInst *MI, int OpNo,
                                        raw_ostream &OS) {
  llvm_unreachable("printCCOperand not implemented yet");
}

void SRRArchInstPrinter::printPredicateOperand(const MCInst *MI, unsigned OpNo,
                                               raw_ostream &OS) {
  llvm_unreachable("printPredicateOperand not implemented yet");
}
