//===-- SRRArchAsmBackend.cpp - SRRArch Assembler Backend -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SRRArchMCTargetDesc.h"
#include "SRRArchFixupKinds.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"

using namespace llvm;

namespace {
class SRRArchAsmBackend : public MCAsmBackend {
  Triple::OSType OSType;

public:
  SRRArchAsmBackend(const Target &T, Triple::OSType OST)
      : MCAsmBackend(llvm::endianness::big), OSType(OST) {}

  void applyFixup(const MCFragment &, const MCFixup &, const MCValue &Target,
                  uint8_t *Data, uint64_t Value, bool IsResolved) override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  MCFixupKindInfo getFixupKindInfo(MCFixupKind Kind) const override;

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

bool SRRArchAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                     const MCSubtargetInfo *STI) const {
  llvm_unreachable("writeNopData not implemented yet");
  return false;
}

void SRRArchAsmBackend::applyFixup(const MCFragment &F, const MCFixup &Fixup,
                                   const MCValue &Target, uint8_t *Data,
                                   uint64_t Value, bool IsResolved) {
  llvm_unreachable("applyFixup not implemented yet");
}

std::unique_ptr<MCObjectTargetWriter>
SRRArchAsmBackend::createObjectTargetWriter() const {
  return createSRRArchELFObjectWriter(
      MCELFObjectTargetWriter::getOSABI(OSType));
}

MCFixupKindInfo SRRArchAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  llvm_unreachable("getFixupKindInfo not implemented yet");
  return MCAsmBackend::getFixupKindInfo(Kind);
}

} // namespace

MCAsmBackend *
llvm::createSRRArchAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                              const MCRegisterInfo & /*MRI*/,
                              const MCTargetOptions & /*Options*/) {
  const Triple &TT = STI.getTargetTriple();
  if (!TT.isOSBinFormatELF())
    llvm_unreachable("OS not supported");

  return new SRRArchAsmBackend(T, TT.getOS());
}
