#pragma once
#include <idafucker/CoreDefines.hpp>

#if (IDAFUCKER_PLATFORM_WIN32)
  #include "windows/WindowsWindow.hpp"
#else // IDAFUCKER_PLATFORM_LINUX
  #pragma error("unimplemented")
#endif // IDAFUCKER_PLATFORM_WIN32

IDAFUCKER_NAMESPACE_BEGIN

#if (IDAFUCKER_PLATFORM_WIN32)
using Window = WindowsAppWindow;
#else // IDAFUCKER_PLATFORM_LINUX
#endif // IDAFUCKER_PLATFORM_WIN32

IDAFUCKER_NAMESPACE_END