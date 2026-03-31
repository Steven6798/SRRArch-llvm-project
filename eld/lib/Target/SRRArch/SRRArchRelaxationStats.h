//===-SRRArchRelaxationStats.h---------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef SRRArch_RELAXATION_STATS_H
#define SRRArch_RELAXATION_STATS_H

#include "eld/LayoutMap/LinkStats.h"

namespace eld {
class SRRArchRelaxationStats : public LinkStats {
public:
  SRRArchRelaxationStats()
      : LinkStats("RelaxationStats", LinkStats::Kind::Relaxation) {}

  void dumpStat(llvm::raw_ostream &OS) const override;

private:
};
} // namespace eld

#endif
