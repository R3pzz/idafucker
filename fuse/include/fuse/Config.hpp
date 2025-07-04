#pragma once

// Disable the implicitly deleted destructor warning.
#pragma warning(disable : 4624)

// A minimal set of includes a well-structured project should have.
#include <cassert>
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

// Complier/linker-specific macros
#if defined(_MSC_VER)
  // When a method is declared as "naked", the compiler does not geneterate
  // a prologue and an epilogue for it, meaning that it will only contain the
  // instructions you've put in the function body.
  #if not defined(FUSE_NAKED)
    #define FUSE_NAKED __declspec(naked)
  #endif

  // Prohibit the compiler from inlining the following method.
  #if not defined(FUSE_NOINLINE)
    #define FUSE_NOINLINE __declspec(noinline)
  #endif

  // Windows API-specific calling convention.
  #if not defined(FUSE_STDCALL)
    #define FUSE_STDCALL __stdcall
  #endif

  // Sometimes, we would want to constrain the linker to put some code
  // into the region we want it to be.
  #if not defined(FUSE_SPECIFY_SECTION)
    #define FUSE_SPECIFY_SECTION(section) __declspec(allocate(section))
  #endif
#endif