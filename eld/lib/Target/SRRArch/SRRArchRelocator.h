//===-SRRArchRelocator.h---------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef SRRArch_RELOCATION_FACTORY_H
#define SRRArch_RELOCATION_FACTORY_H

#include "SRRArchLDBackend.h"
#include "SRRArchRelocationInternal.h"
#include "eld/Target/Relocator.h"
#include <mutex>

namespace eld {

class ResolveInfo;
class LinkerConfig;

/** \class SRRArchRelocator
 *  \brief SRRArchRelocator creates and destroys the SRRArch relocations.
 *
 */
class SRRArchRelocator : public Relocator {
public:
  SRRArchRelocator(SRRArchLDBackend &pParent, LinkerConfig &pConfig,
                   Module &pModule);
  ~SRRArchRelocator();

  Result applyRelocation(Relocation &pRelocation) override;

  void scanRelocation(Relocation &pReloc, eld::IRBuilder &pBuilder,
                      ELFSection &pSection, InputFile &pInput,
                      CopyRelocs &) override;

  // Handle partial linking
  void partialScanRelocation(Relocation &pReloc,
                             const ELFSection &pSection) override;

  SRRArchLDBackend &getTarget() override;

  const SRRArchLDBackend &getTarget() const override;

  const char *getName(Relocation::Type pType) const override;

  uint32_t getNumRelocs() const override;

  Size getSize(Relocation::Type pType) const override;

private:


public:
  SRRArchLDBackend &m_Target;

private:
  std::mutex m_RelocMutex;
};

} // namespace eld

#endif
