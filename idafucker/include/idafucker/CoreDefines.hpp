#pragma once

// PLATFORM DETECTION
#if defined(_WIN32)
  #define IDAFUCKER_PLATFORM_WIN32 (true)
#elif defined(__LINUX__)
  #define IDAFUCKER_PLATFORM_LINUX (true)
#endif // defined(_WIN32)

// PLATFORM HEADER INCLUDES
#if not defined(WIN32_LEAN_AND_MEAN)
  #define WIN32_LEAN_AND_MEAN
#endif // not defined(WIN32_LEAN_AND_MEAN)
#if not defined(NOMINMAX)
  #define NOMINMAX
#endif // not defined(NOMINMAX)
#include <Windows.h>

// NAMESPACES
#if not defined(IDAFUCKER_NAMESPACE_BEGIN)
  #define IDAFUCKER_NAMESPACE_BEGIN \
    namespace idafucker             \
    {
#endif // not defined(IDAFUCKER_NAMESPACE_BEGIN)

#if not defined(IDAFUCKER_NAMESPACE_END)
  #define IDAFUCKER_NAMESPACE_END }
#endif // not defined(IDAFUCKER_NAMESPACE_END)

// CLASS HELPERS
#if not defined(IDAFUCKER_NONCOPYABLE)
  #define IDAFUCKER_NONCOPYABLE(x) \
    x(const x &) = delete;         \
    auto operator=(const x &) = delete;
#endif // not defined(IDAFUCKER_NONCOPYABLE)
#if not defined(IDAFUCKER_NONMOVABLE)
  #define IDAFUCKER_NONMOVABLE(x) \
    x(x &&) = delete;             \
    auto operator=(x &&) = delete;
#endif // not defined(IDAFUCKER_NONMOVABLE)

// UTILS
#if (IDAFUCKER_PLATFORM_WIN32)
  #define IDAFUCKER_CHECK(x)                              \
    if (x != S_OK) {                                      \
      throw std::runtime_error{"Call to " #x " failed!"}; \
    }
#endif // IDAFUCKER_PLATFORM_WIN32

#if not defined(IDAFUCKER_CHECK_NONZERO)
  #define IDAFUCKER_CHECK_NONZERO(x)                      \
    if (!x) {                                             \
      throw std::runtime_error{"Call to " #x " failed!"}; \
    }
#endif // not defined(IDAFUCKER_CHECK_NONZERO)

#include <cstddef>
#include <cstdlib>
#include <stdexcept>