add_llvm_component_library(LLVMSRRArchInfo
  SRRArchTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  SRRArch
  )
