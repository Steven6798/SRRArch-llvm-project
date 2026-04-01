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
const RelocationInfo SRRArchRelocs[] = {{/*.Name = */ "R_SRRARCH_NONE",
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
                                         /*.Size = */ 64}};
} // extern "C"

#endif
