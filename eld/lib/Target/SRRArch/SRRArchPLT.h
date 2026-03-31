//===-SRRArchPLT.h---------------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef ELD_TARGET_SRRARCH_PLT_H
#define ELD_TARGET_SRRARCH_PLT_H

#include "SRRArchGOT.h"
#include "eld/Fragment/PLT.h"

namespace eld {
class LinkerConfig;
class SRRArchGOT;
class SRRArchLDBackend;

class SRRArchPLT : public PLT {
public:
  SRRArchPLT(PLT::PLTType T, SRRArchGOT *G, ELFSection *P, ResolveInfo *R,
             uint32_t Align, uint32_t Size)
      : PLT(T, G, P, R, Align, Size) {}

  llvm::ArrayRef<uint8_t> getContent() const override {
    return llvm::ArrayRef<uint8_t>();
  }

  virtual eld::Expected<void> emit(MemoryRegion &mr, Module &M) override = 0;

  virtual ~SRRArchPLT() {}
};

template <typename T, uint32_t Align, uint32_t Size>
class SRRArchTPLT : public SRRArchPLT {
public:
  // Going to be used by GOTPLT0
  SRRArchTPLT(PLT::PLTType pltType, SRRArchGOT *G, ELFSection *O,
              ResolveInfo *R)
      : SRRArchPLT(pltType, G, O, R, Align, Size) {}

  virtual ~SRRArchTPLT() {}
};

template <typename T, uint32_t Align, uint32_t Size>
class SRRArchPLT0 : public SRRArchTPLT<T, Align, Size> {
public:
  SRRArchPLT0(SRRArchGOT *G, SRRArchLDBackend &Backend, ELFSection *P)
      : SRRArchTPLT<T, Align, Size>(PLT::PLT0, G, P, nullptr),
        m_Backend(Backend) {}

  virtual ~SRRArchPLT0() {}

  eld::Expected<void> emit(MemoryRegion &mr, Module &M) override;

private:
  SRRArchLDBackend &m_Backend;
};

} // namespace eld

#endif
