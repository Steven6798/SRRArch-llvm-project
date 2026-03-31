//===-SRRArchELFDynamic.cpp------------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "SRRArchELFDynamic.h"
#include "SRRArchLDBackend.h"
#include "llvm/BinaryFormat/ELF.h"

using namespace eld;

SRRArchELFDynamic::SRRArchELFDynamic(GNULDBackend &pParent,
                                     LinkerConfig &pConfig)
    : ELFDynamic(pParent, pConfig) {}

SRRArchELFDynamic::~SRRArchELFDynamic() {}

void SRRArchELFDynamic::reserveTargetEntries() {
  llvm_unreachable("reserveTargetEntries not implemented yet.");
}

void SRRArchELFDynamic::applyTargetEntries() {
  llvm_unreachable("applyTargetEntries not implemented yet.");
}
