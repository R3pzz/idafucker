#pragma once
#include <CoreDefines.hpp>

#if (IDAFUCKER_PLATFORM_WIN32)
  #include "windows/WindowsApplication.hpp"
#else // IDAFUCKER_PLATFORM_LINUX
  #pragma error("unimplemented")
#endif // IDAFUCKER_PLATFORM_WIN32

IDAFUCKER_NAMESPACE_BEGIN

#if (IDAFUCKER_PLATFORM_WIN32)
using App = WindowsApp;
#else // IDAFUCKER_PLATFORM_LINUX
  #pragma error("unimplemented")
#endif // IDAFUCKER_PLATFORM_WIN32

IDAFUCKER_NAMESPACE_END