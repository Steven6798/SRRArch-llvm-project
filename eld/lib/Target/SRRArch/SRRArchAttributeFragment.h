//===-SRRArchAttributeFragment.h-------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef ELD_FRAGMENT_SRRARCH_ATTRIBUTE_FRAGMENT_H
#define ELD_FRAGMENT_SRRARCH_ATTRIBUTE_FRAGMENT_H

#include "eld/Fragment/TargetFragment.h"
#include "eld/Readers/ELFSection.h"
#include "llvm/ADT/SmallVector.h"
#include <string>
#include <vector>

namespace llvm {
class SRRArchAttributeParser;
}

namespace eld {

class DiagEngine;
class InputFile;
class GNULDBackend;
class LinkerConfig;

class SRRArchAttributeFragment : public TargetFragment {
public:
  SRRArchAttributeFragment(ELFSection *O);

  virtual ~SRRArchAttributeFragment();

  /// name - name of this stub
  virtual const std::string name() const override;

  virtual size_t size() const override;

  virtual eld::Expected<void> emit(MemoryRegion &mr, Module &M) override;

  bool updateInfo(GNULDBackend *G) override;

  virtual void dump(llvm::raw_ostream &OS) override;

private:
protected:
};

} // namespace eld

#endif
