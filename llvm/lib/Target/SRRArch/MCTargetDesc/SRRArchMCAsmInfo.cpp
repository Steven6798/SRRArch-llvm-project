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

using namespace llvm;

void SRRArchMCAsmInfo::anchor() {}

SRRArchMCAsmInfo::SRRArchMCAsmInfo(const Triple & /*TheTriple*/,
                                   const MCTargetOptions &Options) {
  IsLittleEndian = true;
  ExceptionsType = ExceptionHandling::DwarfCFI;
  UsesELFSectionDirectiveForBSS = true;

  // Target supports emission of debugging information.
  SupportsDebugInformation = true;

  Data8bitsDirective = "\t.8bit\t";
  Data16bitsDirective = "\t.16bit\t";
  Data32bitsDirective = "\t.32bit\t";
  Data64bitsDirective = "\t.64bit\t";
  MinInstAlignment = 8;
  MaxInstLength = 8;
}

void SRRArchMCAsmInfo::printSpecifierExpr(raw_ostream &OS,
                                          const MCSpecifierExpr &Expr) const {
  llvm_unreachable("printSpecifierExpr not implemented yet");
}
