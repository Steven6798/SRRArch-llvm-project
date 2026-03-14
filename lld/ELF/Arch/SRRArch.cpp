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

static void writeGVBLAddress(uint8_t *loc, uint64_t val) {
  // The 32-bit value needs to be placed starting at bit 13
  uint64_t v = (uint64_t)val << 13;

  // Save the register field (bits 0-4 of byte 1)
  uint8_t reg_field = loc[1] & 0x1F; // Preserve low 5 bits

  // Write bytes 1-5 (bits 8-47)
  for (int i = 1; i <= 5; i++) {
    loc[i] = (v >> (i * 8)) & 0xFF;
  }

  // Restore the register field
  loc[1] = (loc[1] & ~0x1F) | reg_field;
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

    writeGVBLAddress(loc, val);
    break;
  }
  case R_SRRARCH_BRANCH: {
    checkUInt(ctx, loc, val, 32, rel);

    // unconditional branch
    if (loc[0] == 0x17) {
      write32le(loc + 1, val);
    } else {
      writeGVBLAddress(loc, val);
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
  case R_SRRARCH_BRANCH:
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
