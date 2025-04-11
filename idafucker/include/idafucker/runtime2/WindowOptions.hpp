#pragma once
#include <idafucker/CoreDefines.hpp>

#if defined(IDAFUCKER_PLATFORM_WIN32)
  #include "impl/win/WindowOptionsImpl.hpp"
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)

IDAFUCKER_NAMESPACE_BEGIN

#if defined(IDAFUCKER_PLATFORM_WIN32)
using WindowOptions = impl::win::WindowOptionsImpl;
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)

IDAFUCKER_NAMESPACE_END