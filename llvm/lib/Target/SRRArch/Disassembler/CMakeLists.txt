add_llvm_component_library(LLVMSRRArchDisassembler
  SRRArchDisassembler.cpp

  LINK_COMPONENTS
  CodeGenTypes
  SRRArchDesc
  SRRArchInfo
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  SRRArch
  )
