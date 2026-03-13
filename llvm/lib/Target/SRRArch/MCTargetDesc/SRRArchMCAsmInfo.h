//=====-- SRRArchMCAsmInfo.h - SRRArch asm properties ---------*- C++ -*--====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the SRRArchMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHMCASMINFO_H
#define LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class SRRArchMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit SRRArchMCAsmInfo(const Triple &TheTriple,
                            const MCTargetOptions &Options);
  void printSpecifierExpr(raw_ostream &OS,
                          const MCSpecifierExpr &Expr) const override;
};

namespace SRRArch {
using Specifier = uint8_t;
enum { S_None };
} // namespace SRRArch

} // namespace llvm

#endif // LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHMCASMINFO_H
