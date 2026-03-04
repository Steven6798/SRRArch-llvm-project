add_llvm_component_library(LLVMSRRArchDesc
  SRRArchAsmBackend.cpp
  SRRArchELFObjectWriter.cpp
  SRRArchInstPrinter.cpp
  SRRArchMCAsmInfo.cpp
  SRRArchMCCodeEmitter.cpp
  SRRArchMCTargetDesc.cpp

  LINK_COMPONENTS
  SRRArchInfo
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  SRRArch
)
