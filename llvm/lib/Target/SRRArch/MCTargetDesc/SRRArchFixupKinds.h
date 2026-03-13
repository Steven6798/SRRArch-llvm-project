//===-- SRRArchFixupKinds.h - SRRArch Specific Fixup Entries ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHFIXUPKINDS_H
#define LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace SRRArch {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the save order of
// MCFixupKindInfo Infos[SRRArch::NumTargetFixupKinds]
// in SRRArchAsmBackend.cpp.
//
enum Fixups {
  // Results in R_SRRArch_NONE
  FIXUP_SRRARCH_NONE = FirstTargetFixupKind,

  FIXUP_SRRARCH_32, // general 32-bit relocation
  FIXUP_SRRARCH_64, // general 64-bit relocation
  FIXUP_SRRARCH_GV, // 32-bit global targets

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace SRRArch
} // namespace llvm

#endif // LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHFIXUPKINDS_H
