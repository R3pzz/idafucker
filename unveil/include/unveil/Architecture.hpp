#pragma once
#include <idafucker/CoreDefines.hpp>

#include <Zydis/SharedTypes.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
enum class Architecture {
  x86,    //< x86/i386 ISA.
  Amd64,  //< amd64/x86-64 ISA.
};

namespace detail
{
template <Architecture> struct ZydisArchitectureAdapter;

template <> struct ZydisArchitectureAdapter<Architecture::x86> {
  static constexpr auto MachMode{ZYDIS_MACHINE_MODE_LEGACY_32};
  static constexpr auto AddrWidth{ZYDIS_ADDRESS_WIDTH_32};
};

template <> struct ZydisArchitectureAdapter<Architecture::Amd64> {
  static constexpr auto MachMode{ZYDIS_MACHINE_MODE_LONG_64};
  static constexpr auto AddrWidth{ZYDIS_ADDRESS_WIDTH_64};
};
} // namespace detail
} // namespace disasm

IDAFUCKER_NAMESPACE_END