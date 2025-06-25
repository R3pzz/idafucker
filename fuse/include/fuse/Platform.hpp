#pragma once
#include <fuse/Config.hpp>

#if FUSE_PLATFORM_WIN
  #if not defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif
  #if not defined(NOMINMAX)
    #define NOMINMAX
  #endif
  #include <Windows.h>
#endif