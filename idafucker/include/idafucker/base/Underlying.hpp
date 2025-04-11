#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <typename Enum>
  requires std::is_enum_v<Enum>
[[nodiscard]] constexpr auto underlying(Enum e) noexcept
{
  return static_cast<std::underlying_type_t<Enum>>(e);
}

IDAFUCKER_NAMESPACE_END