//===-SRRArchEmulation.cpp-------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "SRRArch.h"
#include "eld/Config/LinkerConfig.h"
#include "eld/Core/LinkerScript.h"
#include "eld/Support/TargetRegistry.h"
#include "eld/Target/ELFEmulation.h"
#include "llvm/ADT/StringSwitch.h"

using namespace llvm;

namespace eld {

static bool ELDEmulateSRRArchELF(LinkerScript &pScript, LinkerConfig &pConfig) {
  pConfig.targets().setEndian(TargetOptions::Little);
  pConfig.targets().setBitClass(64);

  if (!ELDEmulateELF(pScript, pConfig))
    return false;

  return true;
}

//===----------------------------------------------------------------------===//
// emulateSRRArchLD - the help function to emulate SRRArch ld
//===----------------------------------------------------------------------===//
bool emulateSRRArchLD(LinkerScript &pScript, LinkerConfig &pConfig) {
  return ELDEmulateSRRArchELF(pScript, pConfig);
}

} // namespace eld

//===----------------------------------------------------------------------===//
// SRRArchEmulation
//===----------------------------------------------------------------------===//
extern "C" void ELDInitializeSRRArchEmulation() {
  // Register the emulation
  eld::TargetRegistry::RegisterEmulation(eld::TheSRRArchTarget,
                                         eld::emulateSRRArchLD);
}
