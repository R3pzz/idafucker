#pragma once
#include <idafucker/CoreDefines.hpp>

#if defined(UNICODE)
  #define WINDOWS_STRING(str) L##str
#else
  #define WINDOWS_STRING(str) str
#endif  // defined(UNICODE)

IDAFUCKER_NAMESPACE_BEGIN

namespace win::detail {
#if defined(UNICODE)
using Char = const wchar_t*;
#else
using Char = const char*;
#endif  // defined(UNICODE)
}  // namespace win::detail

IDAFUCKER_NAMESPACE_END