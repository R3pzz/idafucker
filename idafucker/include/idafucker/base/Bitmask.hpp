#pragma once
#include <idafucker/CoreDefines.hpp>

#include "Underlying.hpp"

IDAFUCKER_NAMESPACE_BEGIN

template <typename Type>
concept scoped_enum = std::is_scoped_enum_v<Type>;

IDAFUCKER_NAMESPACE_END

template <idafucker::scoped_enum T>
[[nodiscard]] constexpr auto operator|(const T lhs, const T rhs) noexcept
{
  return static_cast<T>(
      idafucker::underlying(lhs) | idafucker::underlying(rhs));
}

template <idafucker::scoped_enum T>
[[nodiscard]] constexpr auto operator&(const T lhs, const T rhs) noexcept
{
  return static_cast<T>(
      idafucker::underlying(lhs) & idafucker::underlying(rhs));
}

template <idafucker::scoped_enum T>
[[nodiscard]] constexpr auto operator^(const T lhs, const T rhs) noexcept
{
  return static_cast<T>(
      idafucker::underlying(lhs) ^ idafucker::underlying(rhs));
}

template <idafucker::scoped_enum T>
[[nodiscard]] constexpr auto operator~(const T value) noexcept
{
  return static_cast<T>(~idafucker::underlying(value));
}