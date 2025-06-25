#pragma once
#include <unveil/Config.hpp>

namespace unveil
{

enum class Architecture {
  None,
  x86,    //< x86/i386/IA32 ISA.
  x86_64,  //< x86-64/amd64 ISA.
};

template <Architecture> struct ArchitectureFormat;

template <> struct ArchitectureFormat<Architecture::x86> {
  using Address = std::uint32_t;
};

template <> struct ArchitectureFormat<Architecture::x86_64> {
  using Address = std::uint64_t;
};

} // namespace unveil