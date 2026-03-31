//===-SRRArchAttributeFragment.cpp-----------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SRRArchAttributeFragment.h"
#include "eld/Diagnostics/DiagnosticEngine.h"
#include "eld/Target/GNULDBackend.h"
#include "llvm/Support/ELFAttributes.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/LEB128.h"

using namespace eld;

//===----------------------------------------------------------------------===//
// SRRArchAttributeFragment
//===----------------------------------------------------------------------===//

SRRArchAttributeFragment::SRRArchAttributeFragment(ELFSection *O)
    : TargetFragment(TargetFragment::Kind::Attributes, O, nullptr,
                     O->getAddrAlign(), 0) {}

SRRArchAttributeFragment::~SRRArchAttributeFragment() {}

const std::string SRRArchAttributeFragment::name() const {
  return "Fragment for SRRArch Attributes";
}

size_t SRRArchAttributeFragment::size() const {
  llvm_unreachable("size not implemented yet.");
}

eld::Expected<void> SRRArchAttributeFragment::emit(MemoryRegion &mr,
                                                   Module &M) {
  llvm_unreachable("emit not implemented yet.");
}

bool SRRArchAttributeFragment::updateInfo(GNULDBackend *G) {
  llvm_unreachable("updateInfo not implemented yet.");
}

void SRRArchAttributeFragment::dump(llvm::raw_ostream &OS) {
  llvm_unreachable("dump not implemented yet.");
}