llvm_add_library(
  ELDSRRArchLDBackend
  STATIC
  SRRArchAttributeFragment.cpp
  SRRArchELFDynamic.cpp
  SRRArchEmulation.cpp
  SRRArchGOT.cpp
  SRRArchInfo.cpp
  SRRArchPLT.cpp
  SRRArchLDBackend.cpp
  SRRArchRelocationCompute.cpp
  SRRArchRelocator.cpp)

add_subdirectory(TargetInfo)

target_link_libraries(
  ELDSRRArchLDBackend PRIVATE ELDSRRArchTargetInfo ELDCore ELDFragment ELDObject
                            ELDTarget LLVMSRRArchCodeGen)
