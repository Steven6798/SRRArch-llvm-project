include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMSRRArchAsmParser
  SRRArchAsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  SRRArchDesc
  SRRArchInfo
  MC
  MCParser
  Support

  ADD_TO_COMPONENT
  SRRArch
  )

add_dependencies( LLVMSRRArchAsmParser SRRArchCommonTableGen )
