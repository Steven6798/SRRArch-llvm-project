//===-- SRRArchELFObjectWriter.cpp - SRRArch ELF Writer -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SRRArchBaseInfo.h"
#include "MCTargetDesc/SRRArchFixupKinds.h"
#include "llvm/MC/MCELFObjectWriter.h"

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
    : MCELFObjectTargetWriter(/*Is64Bit_=*/true, OSABI, ELF::EM_SRRARCH,
                              /*HasRelocationAddend_=*/true) {}

unsigned SRRArchELFObjectWriter::getRelocType(const MCFixup &Fixup,
                                              const MCValue &, bool) const {
  unsigned Type;
  unsigned Kind = static_cast<unsigned>(Fixup.getKind());
  switch (Kind) {
  case SRRArch::FIXUP_SRRARCH_GV:
    Type = ELF::R_SRRARCH_GV;
    break;
  case SRRArch::FIXUP_SRRARCH_BRANCH:
    Type = ELF::R_SRRARCH_BRANCH;
    break;
  case SRRArch::FIXUP_SRRARCH_32:
  case FK_Data_4:
    Type = ELF::R_SRRARCH_32;
    break;
  case SRRArch::FIXUP_SRRARCH_64:
  case FK_Data_8:
    Type = ELF::R_SRRARCH_64;
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
  return false;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createSRRArchELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<SRRArchELFObjectWriter>(OSABI);
}
