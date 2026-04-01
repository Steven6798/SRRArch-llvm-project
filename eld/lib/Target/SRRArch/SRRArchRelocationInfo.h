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
                                         /*.Size = */ 0}};
} // extern "C"

#endif
