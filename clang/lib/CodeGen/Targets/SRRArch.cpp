//===- SRRArch.cpp---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ABIInfoImpl.h"
#include "TargetInfo.h"

using namespace clang;
using namespace clang::CodeGen;

//===----------------------------------------------------------------------===//
// SRRArch ABI Implementation
//===----------------------------------------------------------------------===//

namespace {
class SRRArchABIInfo : public DefaultABIInfo {
  struct CCState {
    unsigned FreeRegs;
  };

public:
  SRRArchABIInfo(CodeGen::CodeGenTypes &CGT) : DefaultABIInfo(CGT) {}

  void computeInfo(CGFunctionInfo &FI) const override {
    CCState State;
    // SRRArch uses 4 registers to pass arguments unless the function has the
    // regparm attribute set.
    if (FI.getHasRegParm()) {
      State.FreeRegs = FI.getRegParm();
    } else {
      State.FreeRegs = 4;
    }

    if (!getCXXABI().classifyReturnType(FI))
      FI.getReturnInfo() = classifyReturnType(FI.getReturnType());
    for (auto &I : FI.arguments())
      I.info = classifyArgumentType(I.type, State);
  }

  ABIArgInfo getIndirectResult(QualType Ty, bool ByVal, CCState &State) const;
  ABIArgInfo classifyArgumentType(QualType RetTy, CCState &State) const;
};
} // end anonymous namespace

ABIArgInfo SRRArchABIInfo::getIndirectResult(QualType Ty, bool ByVal,
                                             CCState &State) const {
  if (!ByVal) {
    if (State.FreeRegs) {
      --State.FreeRegs; // Non-byval indirects just use one pointer.
      return getNaturalAlignIndirectInReg(Ty);
    }
    return getNaturalAlignIndirect(Ty, getDataLayout().getAllocaAddrSpace(),
                                   false);
  }

  // Compute the byval alignment.
  const unsigned MinABIStackAlignInBytes = 8;
  unsigned TypeAlign = getContext().getTypeAlign(Ty) / 8;
  return ABIArgInfo::getIndirect(
      CharUnits::fromQuantity(8),
      /*AddrSpace=*/getDataLayout().getAllocaAddrSpace(), /*ByVal=*/true,
      /*Realign=*/TypeAlign > MinABIStackAlignInBytes);
}

ABIArgInfo SRRArchABIInfo::classifyArgumentType(QualType Ty,
                                                CCState &State) const {
  // Check with the C++ ABI first.
  const RecordType *RT = Ty->getAsCanonical<RecordType>();
  if (RT) {
    CGCXXABI::RecordArgABI RAA = getRecordArgABI(RT, getCXXABI());
    if (RAA == CGCXXABI::RAA_Indirect) {
      return getIndirectResult(Ty, /*ByVal=*/false, State);
    } else if (RAA == CGCXXABI::RAA_DirectInMemory) {
      return getNaturalAlignIndirect(
          Ty, /*AddrSpace=*/getDataLayout().getAllocaAddrSpace(),
          /*ByVal=*/true);
    }
  }

  if (isAggregateTypeForABI(Ty)) {
    // Structures with flexible arrays are always indirect.
    if (RT && RT->getDecl()->getDefinitionOrSelf()->hasFlexibleArrayMember())
      return getIndirectResult(Ty, /*ByVal=*/true, State);

    // Ignore empty structs/unions.
    if (isEmptyRecord(getContext(), Ty, true))
      return ABIArgInfo::getIgnore();

    llvm::LLVMContext &LLVMContext = getVMContext();
    unsigned SizeInRegs = (getContext().getTypeSize(Ty) + 63) / 64;
    if (SizeInRegs <= State.FreeRegs) {
      llvm::IntegerType *Int64 = llvm::Type::getInt64Ty(LLVMContext);
      SmallVector<llvm::Type *, 3> Elements(SizeInRegs, Int64);
      llvm::Type *Result = llvm::StructType::get(LLVMContext, Elements);
      State.FreeRegs -= SizeInRegs;
      return ABIArgInfo::getDirect(Result);
    } else {
      State.FreeRegs = 0;
    }
    return getIndirectResult(Ty, true, State);
  }

  // Treat an enum type as its underlying type.
  if (const auto *ED = Ty->getAsEnumDecl())
    Ty = ED->getIntegerType();

  // Don't pass >64 bit integers in registers.
  if (const auto *EIT = Ty->getAs<BitIntType>())
    if (EIT->getNumBits() > 64)
      return getIndirectResult(Ty, /*ByVal=*/true, State);

  if (isPromotableIntegerTypeForABI(Ty))
    return ABIArgInfo::getExtend(Ty);

  return ABIArgInfo::getDirect();
}

namespace {
class SRRArchTargetCodeGenInfo : public TargetCodeGenInfo {
public:
  SRRArchTargetCodeGenInfo(CodeGen::CodeGenTypes &CGT)
      : TargetCodeGenInfo(std::make_unique<SRRArchABIInfo>(CGT)) {}
};
} // namespace

std::unique_ptr<TargetCodeGenInfo>
CodeGen::createSRRArchTargetCodeGenInfo(CodeGenModule &CGM) {
  return std::make_unique<SRRArchTargetCodeGenInfo>(CGM.getTypes());
}
