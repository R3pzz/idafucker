#pragma once
#include <bridge/shared/NativeStructure.hpp>
#include <bridge/win/shared/native/UnicodeString.hpp>
#include <fuse/Bits.hpp>
#include <fuse/Memory.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win
{

struct LDR {};

struct CurDir {
  UnicodeString dosPath;
  HANDLE handle;
};

struct RTLUserProcessParameters {
  ULONG maxLenght;
  ULONG Lenght;
  ULONG flags;
  ULONG debugFlags;
  HANDLE consoleHandle;
  ULONG consoleFlags;
  HANDLE standardInput;
  HANDLE standardOutput;
  HANDLE standardError;
  CurDir currentDirectory;
  UnicodeString dllPath;
  UnicodeString imagePathName;
  UnicodeString commandLine;
  PVOID environment;
};

struct PEB : NativeStructure {
  static constexpr auto k_is64Bit = sizeof(Address) == 0x8u;

  static constexpr NativeField<BOOLEAN> k_beingDebugged{0x2u};
  static constexpr NativeField<Address> k_imageBaseAddress{k_is64Bit ? 0x10u
                                                                     : 0x8u};
  static constexpr NativeField<Address> k_processParameters{k_is64Bit ? 0x20u
                                                                      : 0x10u};
};

}  // namespace bridge::win