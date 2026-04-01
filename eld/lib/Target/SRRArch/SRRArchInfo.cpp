//===-SRRArchInfo.cpp------------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "SRRArchInfo.h"
#include "eld/Support/MsgHandling.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/BinaryFormat/ELF.h"

using namespace eld;

std::string SRRArchInfo::flagString(uint64_t flag) const {
  llvm_unreachable("flagString not implemented yet.");
}

llvm::StringRef SRRArchInfo::getOutputMCPU() const {
  return m_Config.targets().getTargetCPU();
}

//===----------------------------------------------------------------------===//
// SRRArchInfo
//===----------------------------------------------------------------------===//
SRRArchInfo::SRRArchInfo(LinkerConfig &pConfig) : TargetInfo(pConfig) {}

bool SRRArchInfo::checkFlags(uint64_t flag, const InputFile *pInputFile,
                             bool hasExecutableSections) {
  // If flag is empty and no executable sections found in the ELF file
  // skip checking for compatibility.
  if (!flag && !hasExecutableSections)
    return true;

  if (!m_OutputFlag)
    m_OutputFlag = flag;

  return true;
}

uint8_t SRRArchInfo::OSABI() const { return llvm::ELF::ELFOSABI_NONE; }

bool SRRArchInfo::InitializeDefaultMappings(Module &pModule) {
  LinkerScript &pScript = pModule.getScript();
  if (pScript.linkerScriptHasSectionsCommand())
    return true;
  if (m_Config.codeGenType() == LinkerConfig::Object)
    return true;
  pScript.sectionMap().insert(".sdata.1", ".sdata");
  pScript.sectionMap().insert(".sdata.2", ".sdata");
  pScript.sectionMap().insert(".sdata.4", ".sdata");
  pScript.sectionMap().insert(".sdata.8", ".sdata");
  pScript.sectionMap().insert(".sdata*", ".sdata");
  pScript.sectionMap().insert(".sdata", ".sdata");
  pScript.sectionMap().insert(".sbss.1", ".sdata");
  pScript.sectionMap().insert(".sbss.2", ".sdata");
  pScript.sectionMap().insert(".sbss.4", ".sdata");
  pScript.sectionMap().insert(".sbss.8", ".sdata");
  pScript.sectionMap().insert(".sbss*", ".sdata");
  pScript.sectionMap().insert(".sbss", ".sdata");
  pScript.sectionMap().insert(".scommon.1", ".sdata");
  pScript.sectionMap().insert(".scommon.1.*", ".sdata");
  pScript.sectionMap().insert(".scommon.2", ".sdata");
  pScript.sectionMap().insert(".scommon.2.*", ".sdata");
  pScript.sectionMap().insert(".scommon.4", ".sdata");
  pScript.sectionMap().insert(".scommon.4.*", ".sdata");
  pScript.sectionMap().insert(".scommon.8", ".sdata");
  pScript.sectionMap().insert(".scommon.8.*", ".sdata");
  pScript.sectionMap().insert(".scommon*", ".sdata");
  pScript.sectionMap().insert(".lita", ".sdata");
  pScript.sectionMap().insert(".lit4", ".sdata");
  pScript.sectionMap().insert(".lit8", ".sdata");
  pScript.sectionMap().insert(".gnu.linkonce.s.*", ".sdata");
  pScript.sectionMap().insert(".gnu.linkonce.sb.*", ".sdata");
  pScript.sectionMap().insert(".gnu.linkonce.sb.*", ".sdata");
  pScript.sectionMap().insert(".gnu.linkonce.la*", ".sdata");
  pScript.sectionMap().insert(".gnu.linkonce.l4*", ".sdata");
  pScript.sectionMap().insert(".gnu.linkonce.l8*", ".sdata");
  // These entries will take precedence over platform-independent ones defined
  // later in TargetInfo::InitializeDefaultMappings.
  if (m_Config.options().hasNow()) {
    pScript.sectionMap().insert(".got", ".got");
    pScript.sectionMap().insert(".got.plt", ".got");
  }
  pScript.sectionMap().insert(".gnu.linkonce.l8*", ".sdata");
  return TargetInfo::InitializeDefaultMappings(pModule);
}
