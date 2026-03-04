//===-- SRRArchELFObjectWriter.cpp - SRRArch ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SRRArchBaseInfo.h"
#include "MCTargetDesc/SRRArchFixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class SRRArchELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit SRRArchELFObjectWriter(uint8_t OSABI);

  ~SRRArchELFObjectWriter() override = default;

protected:
  unsigned getRelocType(const MCFixup &, const MCValue &,
                        bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCValue &, unsigned Type) const override;
};

} // end anonymous namespace

SRRArchELFObjectWriter::SRRArchELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_SRRARCH,
                              /*HasRelocationAddend_=*/true) {}

unsigned SRRArchELFObjectWriter::getRelocType(const MCFixup &Fixup,
                                            const MCValue &, bool) const {
  unsigned Type;
  unsigned Kind = static_cast<unsigned>(Fixup.getKind());
  switch (Kind) {
  case SRRArch::FIXUP_SRRARCH_21:
    Type = ELF::R_SRRARCH_21;
    break;
  case SRRArch::FIXUP_SRRARCH_21_F:
    Type = ELF::R_SRRARCH_21_F;
    break;
  case SRRArch::FIXUP_SRRARCH_25:
    Type = ELF::R_SRRARCH_25;
    break;
  case SRRArch::FIXUP_SRRARCH_32:
  case FK_Data_4:
    Type = ELF::R_SRRARCH_32;
    break;
  case SRRArch::FIXUP_SRRARCH_HI16:
    Type = ELF::R_SRRARCH_HI16;
    break;
  case SRRArch::FIXUP_SRRARCH_LO16:
    Type = ELF::R_SRRARCH_LO16;
    break;
  case SRRArch::FIXUP_SRRARCH_NONE:
    Type = ELF::R_SRRARCH_NONE;
    break;

  default:
    llvm_unreachable("Invalid fixup kind!");
  }
  return Type;
}

bool SRRArchELFObjectWriter::needsRelocateWithSymbol(const MCValue &,
                                                   unsigned Type) const {
  switch (Type) {
  case ELF::R_SRRARCH_21:
  case ELF::R_SRRARCH_21_F:
  case ELF::R_SRRARCH_25:
  case ELF::R_SRRARCH_32:
  case ELF::R_SRRARCH_HI16:
    return true;
  default:
    return false;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createSRRArchELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<SRRArchELFObjectWriter>(OSABI);
}
