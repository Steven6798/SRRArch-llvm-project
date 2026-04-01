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
  llvm_unreachable("InitializeDefaultMappings not implemented yet.");
}
