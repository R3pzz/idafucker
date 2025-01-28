#pragma once
#include <CoreDefines.hpp>

#if (IDAFUCKER_PLATFORM_WIN32)
  #include "windows/WindowsWindow.hpp"
IDAFUCKER_NAMESPACE_BEGIN
using AppWindow = WindowsAppWindow;
IDAFUCKER_NAMESPACE_END
#else // IDAFUCKER_PLATFORM_LINUX
  #pragma error("unimplemented")
#endif // IDAFUCKER_PLATFORM_WIN32
