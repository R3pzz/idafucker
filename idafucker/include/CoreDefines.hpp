#pragma once

#if defined(_WIN32)
  #define IDAFUCKER_PLATFORM_WIN32 (true)
#elif defined(__LINUX__)
  #define IDAFUCKER_PLATFORM_LINUX (true)
#endif // defined(_WIN32)

#include <cstdlib>
#include <cstddef>