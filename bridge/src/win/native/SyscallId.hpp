#pragma once
#include <cstdint>

namespace bridge::win
{

enum class SyscallId : std::uint32_t {
  NtQueryInformationProcess = 0x19u,
  NtQuerySystemInformation = 0x36u,
};

} // namespace bridge::win