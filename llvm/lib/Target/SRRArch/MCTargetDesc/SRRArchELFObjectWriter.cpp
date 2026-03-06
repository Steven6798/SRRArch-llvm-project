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
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_SRRARCH,
                              /*HasRelocationAddend_=*/true) {}

unsigned SRRArchELFObjectWriter::getRelocType(const MCFixup &Fixup,
                                              const MCValue &, bool) const {
  llvm_unreachable("getRelocType not implemented yet");
  return 0;
}

bool SRRArchELFObjectWriter::needsRelocateWithSymbol(const MCValue &,
                                                     unsigned Type) const {
  llvm_unreachable("needsRelocateWithSymbol not implemented yet");
  return false;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createSRRArchELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<SRRArchELFObjectWriter>(OSABI);
}
