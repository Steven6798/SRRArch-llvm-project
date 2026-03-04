add_llvm_component_group(SRRArch)

set(LLVM_TARGET_DEFINITIONS SRRArch.td)

tablegen(LLVM SRRArchGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM SRRArchGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SRRArchGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SRRArchGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SRRArchGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM SRRArchGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SRRArchGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM SRRArchGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SRRArchGenSDNodeInfo.inc -gen-sd-node-info)
tablegen(LLVM SRRArchGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(SRRArchCommonTableGen)

add_llvm_target(SRRArchCodeGen
  SRRArchAsmPrinter.cpp
  SRRArchDelaySlotFiller.cpp
  SRRArchFrameLowering.cpp
  SRRArchInstrInfo.cpp
  SRRArchISelDAGToDAG.cpp
  SRRArchISelLowering.cpp
  SRRArchMachineFunctionInfo.cpp
  SRRArchMCInstLower.cpp
  SRRArchMemAluCombiner.cpp
  SRRArchRegisterInfo.cpp
  SRRArchSelectionDAGInfo.cpp
  SRRArchSubtarget.cpp
  SRRArchTargetMachine.cpp
  SRRArchTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  SRRArchAsmParser
  SRRArchDesc
  SRRArchInfo
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  SRRArch
  )

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
