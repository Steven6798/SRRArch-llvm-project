//===-SRRArchStandaloneInfo.h----------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef ELD_TARGET_SRRARCH_STANDALONE_INFO_H
#define ELD_TARGET_SRRARCH_STANDALONE_INFO_H
#include "SRRArchInfo.h"
#include "eld/Config/GeneralOptions.h"
#include "eld/Config/TargetOptions.h"
#include "llvm/BinaryFormat/ELF.h"

namespace eld {

class SRRArchStandaloneInfo : public SRRArchInfo {
public:
  SRRArchStandaloneInfo(LinkerConfig &pConfig) : SRRArchInfo(pConfig) {}

  uint64_t startAddr(bool linkerScriptHasSectionsCmd, bool isDynExec,
                     bool loadPhdr) const override {
    llvm_unreachable("startAddr not implemented yet.");
  }

  void initializeAttributes(InputBuilder &pBuilder) override {
    pBuilder.makeBStatic();
    // Warn on mismatch.
    if (!m_Config.options().hasOptionWarnNoWarnMismatch())
      m_Config.options().setWarnMismatch(true);
  }
};

} // namespace eld

#endif
