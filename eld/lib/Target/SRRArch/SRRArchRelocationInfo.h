//===- SRRArchRelocationInfo.h---------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef SRRARCH_RELOCATION_INFO_H
#define SRRARCH_RELOCATION_INFO_H

#include "SRRArchLLVMExtern.h"
#include "llvm/BinaryFormat/ELF.h"

extern "C" {
const RelocationInfo SRRArchRelocs[] = {
    {/*.Name = */ "R_SRRARCH_NONE",
     /*.Type = */ llvm::ELF::R_SRRARCH_NONE,
     /*EncodingType = */ EncTy_None,
     /*.Alignment = */ 0,
     /*.shift = */ 0,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 0},
    {/*.Name = */ "R_SRRARCH_32",
     /*.Type = */ llvm::ELF::R_SRRARCH_32,
     /*EncodingType = */ EncTy_32,
     /*.Alignment = */ 4,
     /*.shift = */ 0,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 32},
    {/*.Name = */ "R_SRRARCH_64",
     /*.Type = */ llvm::ELF::R_SRRARCH_64,
     /*EncodingType = */ EncTy_64,
     /*.Alignment = */ 8,
     /*.shift = */ 0,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 64},
    {/*.Name = */ "R_SRRARCH_GV",
     /*.Type = */ llvm::ELF::R_SRRARCH_GV,
     /*EncodingType = */ EncTy_32,
     /*.Alignment = */ 0,
     /*.shift = */ 12,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 32},
    {/*.Name = */ "R_SRRARCH_BR",
     /*.Type = */ llvm::ELF::R_SRRARCH_BR,
     /*EncodingType = */ EncTy_32,
     /*.Alignment = */ 0,
     /*.shift = */ 8,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 32},
    {/*.Name = */ "R_SRRARCH_BRCOND",
     /*.Type = */ llvm::ELF::R_SRRARCH_BRCOND,
     /*EncodingType = */ EncTy_32,
     /*.Alignment = */ 0,
     /*.shift = */ 12,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 32},
    {/*.Name = */ "R_SRRARCH_CALL",
     /*.Type = */ llvm::ELF::R_SRRARCH_CALL,
     /*EncodingType = */ EncTy_32,
     /*.Alignment = */ 0,
     /*.shift = */ 8,
     /*.VerifyRange = */ false,
     /*.VerifyAlignment = */ false,
     /*.Signed = */ false,
     /*.Size = */ 32},
};
} // extern "C"

#endif
