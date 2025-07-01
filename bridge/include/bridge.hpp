#pragma once
#include <fuse/Config.hpp>

#if FUSE_PLATFORM_WIN
  #include <bridge-win/bridge-win.hpp>
#elif FUSE_PLATFORM_LINUX
  #include <bridge-linux/bridge-linux.hpp>
#else
  #error "Unsupported platform."
#endif

namespace bridge
{

#if FUSE_PLATFORM_WIN
using namespace ::bridge::win;
#elif FUSE_PLATFORM_LINUX
using namespace ::bridge::linux;
#endif

} // namespace bridge