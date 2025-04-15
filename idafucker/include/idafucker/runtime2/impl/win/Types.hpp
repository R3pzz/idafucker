#pragma once
#include <idafucker/CoreDefines.hpp>

#if not defined(UTF8_OR_UNICODE_VARIANT)
  #if defined(UNICODE)
    #define UTF8_OR_UNICODE_VARIANT(utf8, unicode) unicode
  #else
    #define UTF8_OR_UNICODE_VARIANT(utf8, unicode) utf8
  #endif
#endif

IDAFUCKER_NAMESPACE_BEGIN

namespace impl::win
{
using Character = UTF8_OR_UNICODE_VARIANT(char, wchar_t);
using View = std::basic_string_view<Character>;
using String = std::basic_string<Character>;

constexpr auto Delimiter{UTF8_OR_UNICODE_VARIANT(' ', L' ')};
}  // namespace impl::win

IDAFUCKER_NAMESPACE_END