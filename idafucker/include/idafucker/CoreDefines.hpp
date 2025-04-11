#pragma once

// PLATFORM DETECTION
#if defined(_WIN32)
#  define IDAFUCKER_PLATFORM_WIN32 (true)
#elif defined(__LINUX__)
#  define IDAFUCKER_PLATFORM_LINUX (true)
#endif  // defined(_WIN32)

// BUILD VERSION
#if defined(_DEBUG)
#  define IDAFUCKER_DEBUG (true)
#else
#  define IDAFUCKER_RELEASE (true)
#endif  // defined(_DEBUG)

// PLATFORM HEADER INCLUDES
#if not defined(WIN32_LEAN_AND_MEAN)
#  define WIN32_LEAN_AND_MEAN
#endif  // not defined(WIN32_LEAN_AND_MEAN)
#if not defined(NOMINMAX)
#  define NOMINMAX
#endif  // not defined(NOMINMAX)
#include <Windows.h>

// NAMESPACES
#if not defined(IDAFUCKER_NAMESPACE_BEGIN)
#  define IDAFUCKER_NAMESPACE_BEGIN namespace idafucker {
#endif  // not defined(IDAFUCKER_NAMESPACE_BEGIN)

#if not defined(IDAFUCKER_NAMESPACE_END)
#  define IDAFUCKER_NAMESPACE_END }
#endif  // not defined(IDAFUCKER_NAMESPACE_END)

// CLASS HELPERS
#if not defined(IDAFUCKER_NONCOPYABLE)
#  define IDAFUCKER_NONCOPYABLE(x)                                                                 \
    x(const x&) = delete;                                                                          \
    auto operator=(const x&) = delete;
#endif  // not defined(IDAFUCKER_NONCOPYABLE)
#if not defined(IDAFUCKER_NONMOVABLE)
#  define IDAFUCKER_NONMOVABLE(x)                                                                  \
    x(x&&) = delete;                                                                               \
    auto operator=(x&&) = delete;
#endif  // not defined(IDAFUCKER_NONMOVABLE)

// UTILS
#if defined(IDAFUCKER_PLATFORM_WIN32)
// Win32-specific checks
#endif

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

// ARCHITECTURE DETECTION
#if (INTPTR_MAX == INT32_MAX)
#  define IDAFUCKER_ARCHITECTURE_X86 (true)
#  define IDAFUCKER_EACH_ARCH(x86, x64) (x86)
#else
#  define IDAFUCKER_ARCHITECTURE_X64 (true)
#  define IDAFUCKER_EACH_ARCH(x86, x64) (x64)
#endif  // (INTPTR_MAX == INT32_MAX)

// SPDLOG
#if not defined(SPDLOG_USE_STD_FORMAT)
#  define SPDLOG_USE_STD_FORMAT
#endif  // not defined(SPDLOG_USE_STD_FORMAT)