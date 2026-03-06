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
  PrivateGlobalPrefix = ".L";
  WeakRefDirective = "\t.weak\t";
  ExceptionsType = ExceptionHandling::DwarfCFI;

  // SRRArch assembly requires ".section" before ".bss"
  UsesELFSectionDirectiveForBSS = true;

  // Use '!' as comment string to correspond with old toolchain.
  CommentString = "!";

  // Target supports emission of debugging information.
  SupportsDebugInformation = true;

  // Set the instruction alignment. Currently used only for address adjustment
  // in dwarf generation.
  MinInstAlignment = 4;
}

void SRRArchMCAsmInfo::printSpecifierExpr(raw_ostream &OS,
                                          const MCSpecifierExpr &Expr) const {
  llvm_unreachable("printSpecifierExpr not implemented yet");
}
