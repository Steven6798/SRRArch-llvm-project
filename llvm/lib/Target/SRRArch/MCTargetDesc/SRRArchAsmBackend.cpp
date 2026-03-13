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
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"

using namespace llvm;

namespace {
class SRRArchAsmBackend : public MCAsmBackend {
  Triple::OSType OSType;

public:
  SRRArchAsmBackend(const Target &T, Triple::OSType OST)
      : MCAsmBackend(llvm::endianness::little), OSType(OST) {}

  void applyFixup(const MCFragment &, const MCFixup &, const MCValue &Target,
                  uint8_t *Data, uint64_t Value, bool IsResolved) override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  MCFixupKindInfo getFixupKindInfo(MCFixupKind Kind) const override;

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

// Prepare value for the target space
static uint64_t adjustFixupValue(MCFixupKind Kind, uint64_t Value) {
  switch (Kind) {
  case FK_Data_1:
  case FK_Data_2:
  case FK_Data_4:
  case FK_Data_8:
    return Value;
  default:
    llvm_unreachable("Unknown fixup kind!");
  }
}

bool SRRArchAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                     const MCSubtargetInfo *STI) const {
  for (uint64_t I = 0; I < Count; ++I)
    OS << char(SRRArch::NOP);

  return true;
}

void SRRArchAsmBackend::applyFixup(const MCFragment &F, const MCFixup &Fixup,
                                   const MCValue &Target, uint8_t *Data,
                                   uint64_t Value, bool IsResolved) {
  maybeAddReloc(F, Fixup, Target, Value, IsResolved);

  MCFixupKind Kind = Fixup.getKind();
  if (mc::isRelocation(Kind))
    return;

  MCFixupKindInfo Info = getFixupKindInfo(Kind);
  if (!Value)
    return; // Doesn't change encoding.

  // Apply any target-specific value adjustments.
  Value = adjustFixupValue(Kind, Value);

  // Shift the value into position.
  Value <<= Info.TargetOffset;

  unsigned NumBytes = alignTo(Info.TargetSize + Info.TargetOffset, 8) / 8;
  // Where do we start in the object
  unsigned Offset = Fixup.getOffset();
  assert(Offset + NumBytes <= F.getSize() && "Invalid fixup offset!");

  // For each byte of the fragment that the fixup touches, mask in the
  // bits from the fixup value.
  for (unsigned i = 0; i != NumBytes; ++i) {
    Data[i + Offset] |= uint8_t((Value >> (i * 8)) & 0xff);
  }
}

std::unique_ptr<MCObjectTargetWriter>
SRRArchAsmBackend::createObjectTargetWriter() const {
  return createSRRArchELFObjectWriter(
      MCELFObjectTargetWriter::getOSABI(OSType));
}

MCFixupKindInfo SRRArchAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  static const MCFixupKindInfo Infos[SRRArch::NumTargetFixupKinds] = {
      // This table *must* be in same the order of fixup_* kinds in
      // SRRArchFixupKinds.h.
      //
      // name          offset bits flags
      {"FIXUP_SRRARCH_NONE", 0, 32, 0},
      {"FIXUP_SRRARCH_32", 0, 32, 0},
      {"FIXUP_SRRARCH_64", 0, 64, 0},
      {"FIXUP_SRRARCH_GV", 13, 32, 0}};

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < SRRArch::NumTargetFixupKinds &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
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
