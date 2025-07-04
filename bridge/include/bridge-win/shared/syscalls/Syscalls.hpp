#pragma once
#include <bridge-win/shared/syscalls/SyscallInstance.hpp>

namespace bridge::win::syscalls
{

#define BRIDGE_DEFINE_SYSCALL(index, name) constexpr SyscallInstance<index> name{};

BRIDGE_DEFINE_SYSCALL(0x19u, ntQueryInformationProcess);

}  // namespace bridge::win::syscalls