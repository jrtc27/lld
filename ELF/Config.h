//===- Config.h -------------------------------------------------*- C++ -*-===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_CONFIG_H
#define LLD_ELF_CONFIG_H

#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/ELF.h"

#include <vector>

// XXXAR: this should be defined in llvm/Support/Elf.h instead but it's in
// a separate repository
namespace llvm { namespace ELF {
static constexpr uint16_t EM_MIPS_CHERI = 0xC256;
}}

namespace lld {
namespace elf {

class InputFile;
struct Symbol;

enum ELFKind {
  ELFNoneKind,
  ELF32LEKind,
  ELF32BEKind,
  ELF64LEKind,
  ELF64BEKind
};

// For --build-id.
enum class BuildIdKind { None, Fast, Md5, Sha1, Hexstring, Uuid };

// For --discard-{all,locals,none} and --retain-symbols-file.
enum class DiscardPolicy { Default, All, Locals, RetainFile, None };

// For --strip-{all,debug}.
enum class StripPolicy { None, All, Debug };

// For --unresolved-symbols.
enum class UnresolvedPolicy { NoUndef, ReportError, Warn, Ignore };

// For --sort-section and linkerscript sorting rules.
enum class SortSectionPolicy { Default, None, Alignment, Name, Priority };

// For --target2
enum class Target2Policy { Abs, Rel, GotRel };

struct SymbolVersion {
  llvm::StringRef Name;
  bool IsExternCpp;
  bool HasWildcard;
};

// This struct contains symbols version definition that
// can be found in version script if it is used for link.
struct VersionDefinition {
  VersionDefinition(llvm::StringRef Name, uint16_t Id) : Name(Name), Id(Id) {}
  llvm::StringRef Name;
  uint16_t Id;
  std::vector<SymbolVersion> Globals;
  size_t NameOff; // Offset in string table.
};

// This struct contains the global configuration for the linker.
// Most fields are direct mapping from the command line options
// and such fields have the same name as the corresponding options.
// Most fields are initialized by the driver.
struct Configuration {
  InputFile *FirstElf = nullptr;
  uint8_t OSABI = 0;
  llvm::StringMap<uint64_t> SectionStartMap;
  llvm::StringRef DynamicLinker;
  llvm::StringRef Entry;
  llvm::StringRef Emulation;
  llvm::StringRef Fini;
  llvm::StringRef Init;
  llvm::StringRef LTOAAPipeline;
  llvm::StringRef LTONewPmPasses;
  llvm::StringRef MapFile;
  llvm::StringRef OutputFile;
  llvm::StringRef SoName;
  llvm::StringRef Sysroot;
  llvm::StringSet<> RetainSymbolsFile;
  std::string RPath;
  std::vector<VersionDefinition> VersionDefinitions;
  std::vector<llvm::StringRef> AuxiliaryList;
  std::vector<llvm::StringRef> SearchPaths;
  std::vector<llvm::StringRef> SymbolOrderingFile;
  std::vector<llvm::StringRef> Undefined;
  std::vector<SymbolVersion> VersionScriptGlobals;
  std::vector<SymbolVersion> VersionScriptLocals;
  std::vector<uint8_t> BuildIdVector;
  bool AllowMultipleDefinition;
  bool AsNeeded = false;
  bool Bsymbolic;
  bool BsymbolicFunctions;
  bool ColorDiagnostics = false;
  bool DefineCommon;
  bool Demangle = true;
  bool DisableVerify;
  bool EhFrameHdr;
  bool EnableNewDtags;
  bool ExportDynamic;
  bool FatalWarnings;
  bool GcSections;
  bool GdbIndex;
  bool GnuHash = false;
  bool ICF;
  bool Mips64EL = false;
  bool MipsN32Abi = false;
  bool MipsCheriAbi = false;
  bool NoGnuUnique;
  bool NoUndefinedVersion;
  bool Nostdlib;
  bool OFormatBinary;
  bool OMagic;
  bool Pic;
  bool Pie;
  bool PrintGcSections;
  bool Rela;
  bool Relocatable;
  bool SaveTemps;
  bool SingleRoRx;
  bool Shared;
  bool Static = false;
  bool SysvHash = true;
  bool Target1Rel;
  bool Threads;
  bool Trace;
  bool Verbose;
  bool WarnCommon;
  bool WarnMissingEntry;
  bool ZCombreloc;
  bool ZExecstack;
  bool ZNodelete;
  bool ZNow;
  bool ZOrigin;
  bool ZRelro;
  bool ExitEarly;
  bool ZWxneeded;
  DiscardPolicy Discard;
  SortSectionPolicy SortSection;
  StripPolicy Strip = StripPolicy::None;
  UnresolvedPolicy UnresolvedSymbols;
  Target2Policy Target2 = Target2Policy::GotRel;
  BuildIdKind BuildId = BuildIdKind::None;
  ELFKind EKind = ELFNoneKind;
  uint16_t DefaultSymbolVersion = llvm::ELF::VER_NDX_GLOBAL;
  uint16_t EMachine = llvm::ELF::EM_NONE;
  uint64_t ErrorLimit = 20;
  uint64_t WarningLimit = 20;
  uint64_t ImageBase;
  uint64_t MaxPageSize;
  uint64_t ZStackSize;
  unsigned LTOPartitions;
  unsigned LTOO;
  unsigned Optimize;
  unsigned ThinLTOJobs;

  bool isMIPS() const {
    return EMachine == llvm::ELF::EM_MIPS || EMachine == llvm::ELF::EM_MIPS_CHERI;
  }
};

// The only instance of Configuration struct.
extern Configuration *Config;

} // namespace elf
} // namespace lld

#endif
