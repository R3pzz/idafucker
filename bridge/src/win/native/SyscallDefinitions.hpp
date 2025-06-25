#pragma once
#include "Syscall.hpp"

namespace bridge::win
{

#define BRIDGE_DEFINE_SYSCALL(id, name) \
  constexpr Syscall<SyscallId::id> name{};

BRIDGE_DEFINE_SYSCALL(NtQueryInformationProcess, ntQueryInformationProcess);
BRIDGE_DEFINE_SYSCALL(NtQuerySystemInformation, ntQuerySystemInformation);

} // namespace bridge::win