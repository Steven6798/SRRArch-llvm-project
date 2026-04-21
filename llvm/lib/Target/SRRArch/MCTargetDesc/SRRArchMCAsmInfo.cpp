//===-- SRRArchMCAsmInfo.cpp - SRRArch asm properties
//-----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the SRRArchMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "SRRArchMCAsmInfo.h"
#include "llvm/MC/MCExpr.h"

using namespace llvm;

void SRRArchMCAsmInfo::anchor() {}

SRRArchMCAsmInfo::SRRArchMCAsmInfo(const Triple & /*TheTriple*/,
                                   const MCTargetOptions &Options) {
  IsLittleEndian = true;
  ExceptionsType = ExceptionHandling::DwarfCFI;
  UsesELFSectionDirectiveForBSS = true;

  // Target supports emission of debugging information.
  SupportsDebugInformation = true;

  Data16bitsDirective = "\t.2byte\t";
  Data32bitsDirective = "\t.4byte\t";
  Data64bitsDirective = "\t.8byte\t";
  MinInstAlignment = 8;
  MaxInstLength = 8;
}

void SRRArchMCAsmInfo::printSpecifierExpr(raw_ostream &OS,
                                          const MCSpecifierExpr &Expr) const {
  if (Expr.getSpecifier() == 0) {
    printExpr(OS, *Expr.getSubExpr());
    return;
  }

  switch (Expr.getSpecifier()) {
  default:
    llvm_unreachable("Invalid kind!");
  }
}
