//===-- SRRArchMCTargetDesc.h - SRRArch Target Descriptions -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides SRRArch specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHMCTARGETDESC_H
#define LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHMCTARGETDESC_H

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCTargetOptions.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCSubtargetInfo;
class Target;

MCCodeEmitter *createSRRArchMCCodeEmitter(const MCInstrInfo &MCII,
                                          MCContext &Ctx);

MCAsmBackend *createSRRArchAsmBackend(const Target &T,
                                      const MCSubtargetInfo &STI,
                                      const MCRegisterInfo &MRI,
                                      const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter>
createSRRArchELFObjectWriter(uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for SRRArch registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "SRRArchGenRegisterInfo.inc"

// Defines symbolic names for the SRRArch instructions.
#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "SRRArchGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "SRRArchGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_SRRARCH_MCTARGETDESC_SRRARCHMCTARGETDESC_H
