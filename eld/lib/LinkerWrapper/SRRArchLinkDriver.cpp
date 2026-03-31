//===- SRRArchLinkDriver.cpp-----------------------------------------------===//
// Part of the eld Project, under the BSD License
// See https://github.com/qualcomm/eld/LICENSE.txt for license information.
// SPDX-License-Identifier: BSD-3-Clause
//===----------------------------------------------------------------------===//

#include "eld/Driver/SRRArchLinkDriver.h"
#include "eld/Diagnostics/DiagnosticEngine.h"
#include "eld/Support/MsgHandling.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace llvm::opt;
using namespace eld;

#define OPTTABLE_STR_TABLE_CODE
#include "eld/Driver/SRRArchLinkerOptions.inc"
#undef OPTTABLE_STR_TABLE_CODE

#define OPTTABLE_PREFIXES_TABLE_CODE
#include "eld/Driver/SRRArchLinkerOptions.inc"
#undef OPTTABLE_PREFIXES_TABLE_CODE

static constexpr llvm::opt::OptTable::Info infoTable[] = {
#define OPTION(PREFIXES_OFFSET, PREFIXED_NAME_OFFSET, ID, KIND, GROUP, ALIAS,  \
               ALIASARGS, FLAGS, VISIBILITY, PARAM, HELPTEXT,                  \
               HELPTEXTSFORVARIANTS, METAVAR, VALUES, SUBCOMMANDIDS_OFFSET)    \
  LLVM_CONSTRUCT_OPT_INFO(                                                     \
      PREFIXES_OFFSET, PREFIXED_NAME_OFFSET, SRRArchLinkOptTable::ID, KIND,    \
      SRRArchLinkOptTable::GROUP, SRRArchLinkOptTable::ALIAS, ALIASARGS,       \
      FLAGS, VISIBILITY, PARAM, HELPTEXT, HELPTEXTSFORVARIANTS, METAVAR,       \
      VALUES, SUBCOMMANDIDS_OFFSET),
#include "eld/Driver/SRRArchLinkerOptions.inc"
#undef OPTION
};

static Triple ParseEmulation(std::string pEmulation, Triple &triple,
                             DiagnosticEngine *DiagEngine) {
  Triple result = StringSwitch<Triple>(pEmulation)
                      .Case("elf64srrarch", Triple("srrarch", "", "", ""))
                      .Default(Triple("unknown", "", "", ""));
  // Report invalid emulation error for unknown emulation.
  if (result.getArchName() == "unknown")
    DiagEngine->raise(Diag::err_invalid_emulation) << pEmulation << "\n";
  return result;
}

OPT_SRRArchLinkOptTable::OPT_SRRArchLinkOptTable()
    : GenericOptTable(OptionStrTable, OptionPrefixesTable, infoTable) {}

SRRArchLinkDriver *SRRArchLinkDriver::Create(eld::LinkerConfig &C,
                                             std::string InferredArch) {
  return eld::make<SRRArchLinkDriver>(C, InferredArch);
}

SRRArchLinkDriver::SRRArchLinkDriver(eld::LinkerConfig &C,
                                     std::string InferredArch)
    : GnuLdDriver(C, DriverFlavor::SRRArch) {
  Config.targets().setArch(InferredArch);
}

SRRArchLinkDriver *SRRArchLinkDriver::Create(eld::LinkerConfig &C,
                                             bool is64bit) {
  return eld::make<SRRArchLinkDriver>(C, is64bit);
}

SRRArchLinkDriver::SRRArchLinkDriver(eld::LinkerConfig &C, bool is64bit)
    : GnuLdDriver(C, DriverFlavor::SRRArch) {
  Config.targets().setArch("srrarch");
}

std::optional<int>
SRRArchLinkDriver::parseOptions(ArrayRef<const char *> Args,
                                llvm::opt::InputArgList &ArgList) {
  Table = eld::make<OPT_SRRArchLinkOptTable>();
  unsigned missingIndex;
  unsigned missingCount;
  ArgList = Table->ParseArgs(Args.slice(1), missingIndex, missingCount);
  if (missingCount) {
    Config.raise(eld::Diag::error_missing_arg_value)
        << ArgList.getArgString(missingIndex) << missingCount;
    return LINK_FAIL;
  }
  if (ArgList.hasArg(OPT_SRRArchLinkOptTable::help)) {
    Table->printHelp(outs(), Args[0], "SRRArch Linker", false,
                     /*ShowAllAliases=*/true);
    return LINK_SUCCESS;
  }
  if (ArgList.hasArg(OPT_SRRArchLinkOptTable::help_hidden)) {
    Table->printHelp(outs(), Args[0], "SRRArch Linker", true,
                     /*ShowAllAliases=*/true);
    return LINK_SUCCESS;
  }
  if (ArgList.hasArg(OPT_SRRArchLinkOptTable::version)) {
    printVersionInfo();
    return LINK_SUCCESS;
  }
  // --about
  if (ArgList.hasArg(OPT_SRRArchLinkOptTable::about)) {
    printAboutInfo();
    return LINK_SUCCESS;
  }
  // -repository-version
  if (ArgList.hasArg(OPT_SRRArchLinkOptTable::repository_version)) {
    printRepositoryVersion();
    return LINK_SUCCESS;
  }

  Config.options().setUnknownOptions(
      ArgList.getAllArgValues(OPT_SRRArchLinkOptTable::UNKNOWN));

  return {};
}

bool SRRArchLinkDriver::processLTOOptions(
    llvm::lto::Config &Conf, std::vector<std::string> &LLVMOptions) {
  return GnuLdDriver::processLTOOptions<OPT_SRRArchLinkOptTable>(Conf,
                                                                 LLVMOptions);
}

// Start the link step.
int SRRArchLinkDriver::link(llvm::ArrayRef<const char *> Args,
                            llvm::ArrayRef<llvm::StringRef> ELDFlagsArgs) {
  std::vector<const char *> allArgs = getAllArgs(Args, ELDFlagsArgs);
  if (!ELDFlagsArgs.empty())
    Config.raise(eld::Diag::note_eld_flags_without_output_name)
        << llvm::join(ELDFlagsArgs, " ");
  Config.options().setArgs(allArgs);

  //===--------------------------------------------------------------------===//
  // Special functions.
  //===--------------------------------------------------------------------===//
  static int StaticSymbol;
  std::string lfile =
      llvm::sys::fs::getMainExecutable(allArgs[0], &StaticSymbol);
  SmallString<128> lpath(lfile);
  llvm::sys::path::remove_filename(lpath);
  Config.options().setLinkerPath(std::string(lpath));

  //===--------------------------------------------------------------------===//
  // Begin Link preprocessing
  //===--------------------------------------------------------------------===//
  llvm::opt::InputArgList ArgListLocal;
  if (auto Ret = parseOptions(allArgs, ArgListLocal))
    return *Ret;

  // Save parsed options so they can be accessed later as needed. Right now it's
  // only used for LTO options, but can be expanded to track unused aguments.
  auto &ArgList = Config.options().setParsedArgs(std::move(ArgListLocal));
  if (!processLLVMOptions<OPT_SRRArchLinkOptTable>(ArgList))
    return LINK_FAIL;
  if (!processTargetOptions<OPT_SRRArchLinkOptTable>(ArgList))
    return LINK_FAIL;
  if (!processOptions<OPT_SRRArchLinkOptTable>(ArgList))
    return LINK_FAIL;

  if (!ELDFlagsArgs.empty())
    Config.raise(eld::Diag::note_eld_flags)
        << Config.options().outputFileName() << llvm::join(ELDFlagsArgs, " ");

  if (!checkOptions<OPT_SRRArchLinkOptTable>(ArgList))
    return LINK_FAIL;
  if (!overrideOptions<OPT_SRRArchLinkOptTable>(ArgList))
    return LINK_FAIL;
  std::vector<eld::InputAction *> Action;
  if (!createInputActions<OPT_SRRArchLinkOptTable>(ArgList, Action))
    return LINK_FAIL;
  if (!doLink<OPT_SRRArchLinkOptTable>(ArgList, Action))
    return LINK_FAIL;
  return LINK_SUCCESS;
}

// Some command line options or some combinations of them are not allowed.
// This function checks for such errors.
template <class T>
bool SRRArchLinkDriver::checkOptions(llvm::opt::InputArgList &Args) {
  return GnuLdDriver::checkOptions<T>(Args);
}

template <class T>
bool SRRArchLinkDriver::processOptions(llvm::opt::InputArgList &Args) {
  if (!GnuLdDriver::processOptions<T>(Args))
    return false;

  return true;
}

template <class T>
bool SRRArchLinkDriver::createInputActions(
    llvm::opt::InputArgList &Args, std::vector<eld::InputAction *> &actions) {
  return GnuLdDriver::createInputActions<T>(Args, actions);
}

template <class T>
bool SRRArchLinkDriver::processTargetOptions(llvm::opt::InputArgList &Args) {
  bool result = GnuLdDriver::processTargetOptions<T>(Args);
  std::string emulation = Config.options().getEmulation().str();
  // If a specific emulation was requested, apply it now.
  if (!emulation.empty()) {
    llvm::Triple TheTriple = Config.targets().triple();
    Triple EmulationTriple =
        ParseEmulation(emulation, TheTriple, Config.getDiagEngine());
    if (EmulationTriple.getArch() != Triple::UnknownArch) {
      if (EmulationTriple.getArch() == Triple::srrarch)
        Config.targets().setArch("srrarch");
      TheTriple.setArch(EmulationTriple.getArch());
    }
    if (EmulationTriple.getOS() != Triple::UnknownOS)
      TheTriple.setOS(EmulationTriple.getOS());
    if (EmulationTriple.getEnvironment() != Triple::UnknownEnvironment)
      TheTriple.setEnvironment(EmulationTriple.getEnvironment());
    Config.targets().setTriple(TheTriple);
  }
  return result;
}

template <class T>
bool SRRArchLinkDriver::processLLVMOptions(llvm::opt::InputArgList &Args) {
  return GnuLdDriver::processLLVMOptions<T>(Args);
}
