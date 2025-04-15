#pragma once
#include <hyperui/CoreDefines.hpp>

#if defined(IDAFUCKER_PLATFORM_WIN32)
  #include "impl/win/EngineImpl.hpp"
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)

HYPERUI_NAMESPACE_BEGIN

#if defined(IDAFUCKER_PLATFORM_WIN32)
using Engine = impl::win::EngineImpl;
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)

HYPERUI_NAMESPACE_END