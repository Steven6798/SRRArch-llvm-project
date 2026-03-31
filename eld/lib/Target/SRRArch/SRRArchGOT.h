//===-SRRArchGOT.h---------------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef ELD_TARGET_SRRARCH_GOT_H
#define ELD_TARGET_SRRARCH_GOT_H

#include "eld/Fragment/GOT.h"
#include "eld/Support/Memory.h"
#include "eld/Target/GNULDBackend.h"

namespace eld {

/** \class SRRArchGOT
 *  \brief SRRArch Global Offset Table.
 */

class SRRArchGOT : public GOT {
public:
  // Going to be used by GOTPLT0
  SRRArchGOT(GOTType T, ELFSection *O, ResolveInfo *R, uint32_t Align,
             uint32_t Size)
      : GOT(T, O, R, Align, Size) {
    if (O)
      O->addFragmentAndUpdateSize(this);
  }
};

} // namespace eld

#endif
