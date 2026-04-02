//===-SRRArchInfo.h--------------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef ELD_TARGET_SRRARCH_GNU_INFO_H
#define ELD_TARGET_SRRARCH_GNU_INFO_H
#include "eld/Config/TargetOptions.h"
#include "eld/Core/Module.h"
#include "eld/Target/TargetInfo.h"
#include "llvm/BinaryFormat/ELF.h"

namespace eld {

class SRRArchInfo : public TargetInfo {
public:
  SRRArchInfo(LinkerConfig &m_Config);

  uint32_t machine() const override { return llvm::ELF::EM_SRRARCH; }

  std::string getMachineStr() const override { return "SRRArch"; }

  /// flags - the value of ElfXX_Ehdr::e_flags
  uint64_t flags() const override { return m_OutputFlag; }

  uint8_t OSABI() const override;

  bool checkFlags(uint64_t flag, const InputFile *pInputFile,
                  bool hasExecutableSections) override;

  std::string flagString(uint64_t pFlag) const override;

  bool needEhdr(Module &pModule, bool linkerScriptHasSectionsCmd,
                bool isPhdr) override {
    return false & isPhdr;
  }

  bool processNoteGNUSTACK() override { return false; }

  llvm::StringRef getOutputMCPU() const override;

  bool InitializeDefaultMappings(Module &pModule) override;

private:
  uint64_t m_OutputFlag = 0;
};

} // namespace eld

#endif
