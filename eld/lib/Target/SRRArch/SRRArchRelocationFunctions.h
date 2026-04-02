//===- SRRArchRelocationFunctions.h----------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#ifndef SRRARCH_RELOCATION_FUNCTIONS_H
#define SRRARCH_RELOCATION_FUNCTIONS_H

#include "SRRArchLLVMExtern.h"
#include "SRRArchRelocator.h"

namespace eld {

class Relocation;

struct RelocationDescription;

SRRArchRelocator::Result none(Relocation &pReloc, SRRArchRelocator &pParent,
                              RelocationDescription &RelocDesc);
SRRArchRelocator::Result relocAbs(Relocation &pEntry, SRRArchRelocator &pParent,
                                  RelocationDescription &RelocDesc);
SRRArchRelocator::Result unsupport(Relocation &pReloc,
                                   SRRArchRelocator &pParent,
                                   RelocationDescription &RelocDesc);

struct RelocationDescription;

typedef Relocator::Result (*ApplyFunctionType)(
    eld::Relocation &pReloc, eld::SRRArchRelocator &pParent,
    RelocationDescription &pRelocDesc);

struct RelocationDescription {
  // The application function for the relocation.
  const ApplyFunctionType func;
  // The Relocation type, this is just kept for convenience when writing new
  // handlers for relocations.
  const unsigned int type;
  // If the user specified, the relocation to be force verified, the relocation
  // is verified for alignment, truncation errors(only for relocations that take
  // in non signed values, signed values are bound to exceed the number of
  // bits).
  bool forceVerify;
};

struct RelocationDescription SRRARCHRelocDesc[] = {
    {/*.func = */ none,
     /*.type = */ llvm::ELF::R_SRRARCH_NONE,
     /*.forceVerify = */ false},
    {/*.func = */ relocAbs,
     /*.type = */ llvm::ELF::R_SRRARCH_32,
     /*.forceVerify = */ false},
    {/*.func = */ relocAbs,
     /*.type = */ llvm::ELF::R_SRRARCH_64,
     /*.forceVerify = */ false},
    {/*.func = */ relocAbs,
     /*.type = */ llvm::ELF::R_SRRARCH_GV,
     /*.forceVerify = */ false},
    {/*.func = */ relocAbs,
     /*.type = */ llvm::ELF::R_SRRARCH_BR,
     /*.forceVerify = */ false},
    {/*.func = */ relocAbs,
     /*.type = */ llvm::ELF::R_SRRARCH_BRCOND,
     /*.forceVerify = */ false},
    {/*.func = */ relocAbs,
     /*.type = */ llvm::ELF::R_SRRARCH_CALL,
     /*.forceVerify = */ false}};

#define SRRARCH_MAXRELOCS (llvm::ELF::R_SRRARCH_CALL + 1)

} // namespace eld

#endif // SRRARCH_RELOCATION_FUNCTIONS_H
