//===--- SRRArch.cpp - Implement SRRArch target feature support -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements SRRArch TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "SRRArch.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

const char *const SRRArchTargetInfo::GCCRegNames[] = {
    "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10",
    "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19", "r20", "r21",
    "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};

ArrayRef<const char *> SRRArchTargetInfo::getGCCRegNames() const {
  return llvm::ArrayRef(GCCRegNames);
}

bool SRRArchTargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::StringSwitch<bool>(Name).Case("generic", true).Default(false);
}
void SRRArchTargetInfo::fillValidCPUList(
    SmallVectorImpl<StringRef> &Values) const {
  Values.emplace_back("generic");
}

bool SRRArchTargetInfo::setCPU(const std::string &Name) {
  CPU = llvm::StringSwitch<CPUKind>(Name)
            .Case("generic", CK_GENERIC)
            .Default(CK_NONE);

  return CPU != CK_NONE;
}

bool SRRArchTargetInfo::hasFeature(StringRef Feature) const {
  return llvm::StringSwitch<bool>(Feature).Case("srrarch", true).Default(false);
}

void SRRArchTargetInfo::getTargetDefines(const LangOptions &Opts,
                                         MacroBuilder &Builder) const {
  // Define __srrarch_ when building for target srrarch.
  Builder.defineMacro("__srrarch_");

  // Set define for the CPU specified.
  switch (CPU) {
  case CK_GENERIC:
    Builder.defineMacro("__SRRARCH_GENERIC__");
    break;
  case CK_NONE:
    llvm_unreachable("Unhandled target CPU");
  }
}
