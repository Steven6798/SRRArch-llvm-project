//===-SRRArchPLT.cpp-------------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "SRRArchPLT.h"
#include "SRRArchLDBackend.h"
#include "SRRArchRelocationHelper.h"
#include "eld/Core/Module.h"
#include "eld/Readers/ELFSection.h"
#include "eld/Support/Memory.h"
#include "llvm/Support/Endian.h"

using namespace eld;

template <typename T, uint32_t Align, uint32_t Size>
eld::Expected<void> SRRArchPLT0<T, Align, Size>::emit(MemoryRegion &mr,
                                                      Module &M) {
  llvm_unreachable("emit not implemented yet.");
}