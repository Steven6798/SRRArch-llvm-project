//===-SRRArchTargetInfo.cpp------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "eld/Support/Target.h"
#include "eld/Support/TargetRegistry.h"
#include "llvm/Object/ELF.h"

namespace eld {

eld::Target TheSRRArchTarget;

extern "C" void ELDInitializeSRRArchLDTargetInfo() {
  // register into eld::TargetRegistry
  eld::RegisterTarget Y(TheSRRArchTarget, "srrarch", llvm::ELF::EM_SRRARCH,
                        true);
}

} // namespace eld
