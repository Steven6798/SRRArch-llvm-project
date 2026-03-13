//===- SRRArch.cpp --------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InputFiles.h"
#include "Symbols.h"
#include "Target.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Support/Endian.h"

using namespace llvm;
using namespace llvm::object;
using namespace llvm::support::endian;
using namespace llvm::ELF;
using namespace lld;
using namespace lld::elf;

namespace {
class SRRArch : public TargetInfo {
public:
  SRRArch(Ctx &);
  void relocate(uint8_t *loc, const Relocation &rel,
                uint64_t val) const override;
  RelExpr getRelExpr(RelType type, const Symbol &s,
                     const uint8_t *loc) const override;
  // RelType getDynRel(RelType type) const override;
  int64_t getImplicitAddend(const uint8_t *buf, RelType type) const override;
};
} // namespace

SRRArch::SRRArch(Ctx &ctx) : TargetInfo(ctx) {
  // relativeRel = R_AMDGPU_RELATIVE64;
  // gotRel = R_AMDGPU_ABS64;
  // symbolicRel = R_AMDGPU_ABS64;
}

void SRRArch::relocate(uint8_t *loc, const Relocation &rel,
                       uint64_t val) const {
  switch (rel.type) {
  case R_SRRARCH_32:
    write32le(loc, val);
    break;
  case R_SRRARCH_64:
    write64le(loc, val);
    break;
  case R_SRRARCH_GV: {
    checkUInt(ctx, loc, val, 32, rel);
    uint8_t *p = loc + 1; // 12 / 8
    int shift = 4;        // 12 % 8

    uint64_t v = (uint64_t)val << shift;

    for (int i = 0; i < 5; i++) {
      p[i] &= ~(0xFF << (i * 8));
      p[i] |= (v >> (i * 8)) & 0xFF;
    }
    break;
  }
  default:
    llvm_unreachable("unknown relocation");
  }
}

RelExpr SRRArch::getRelExpr(RelType type, const Symbol &s,
                            const uint8_t *loc) const {
  switch (type) {
  case R_SRRARCH_32:
  case R_SRRARCH_64:
  case R_SRRARCH_GV:
    return R_ABS;
  default:
    Err(ctx) << getErrorLoc(ctx, loc) << "unknown relocation (" << type.v
             << ") against symbol " << &s;
    return R_NONE;
  }
}

// RelType SRRArch::getDynRel(RelType type) const {
//   if (type == R_AMDGPU_ABS64)
//     return type;
//   return R_AMDGPU_NONE;
// }

int64_t SRRArch::getImplicitAddend(const uint8_t *buf, RelType type) const {
  switch (type) {
  default:
    InternalErr(ctx, buf) << "cannot read addend for relocation " << type;
    return 0;
  }
}

void elf::setSRRArchTargetInfo(Ctx &ctx) { ctx.target.reset(new SRRArch(ctx)); }
