#pragma once
#include <fuse/Platform.hpp>

namespace bridge::win::process
{

using Pid = DWORD;

constexpr Pid k_badPid{0u};

}  // namespace bridge::win::process