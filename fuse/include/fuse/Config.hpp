#pragma once

// A minimal set of includes a well-structured project should have.
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

// Platform identification.
#if not defined(FUSE_PLATFORM_WIN) && not defined(FUSE_PLATFORM_LINUX)
  #if defined(_WIN64)
    #define FUSE_PLATFORM_WIN (true)
  #elif defined(__LINUX__)
    #define FUSE_PLATFORM_LINUX (true)
  #else
    #error \
        "Unknown/unsupported platform. If you want this project to support" \
      "any other architecture, you are welcome to contribute to the repository!"
  #endif
#endif

// Architecture identification.
#if not defined(FUSE_ARCH_X86_64) && not defined(FUSE_ARCH_ARM64)
  #if defined(_M_X64)
    #define FUSE_ARCH_X86_64 (true)
  #elif defined(_M_ARM64)
    #define FUSE_ARCH_ARM64 (true)
    #error "ARM-64 is not supported yet."
  #else
    #error "Unknown/unsupported architecture."
  #endif
#endif

// Per-architecture value selection macro.
#if not defined(FUSE_ARCH_SWITCH)
  #if defined(FUSE_ARCH_X86_64)
    #define FUSE_ARCH_SWITCH(x86_64_value, arm64_value) x86_64_value
  #elif defined(FUSE_ARCH_ARM64)
    #define FUSE_ARCH_SWITCH(x86_64_value, arm64_value) arm64_value
  #endif
#endif

// Include default platform headers if necessary.
#if defined(FUSE_EXPOSE_SYSTEM_HEADERS)
  #if FUSE_PLATFORM_WIN
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
  #endif
#endif