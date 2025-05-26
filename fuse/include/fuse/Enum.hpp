#pragma once
#include <fuse/Config.hpp>

namespace fuse
{
[[nodiscard]] constexpr auto underlyingCast(auto e) noexcept {
  return static_cast<std::underlying_type_t<decltype(e)>>(e);
}

template <typename Type>
concept scoped_enum = std::is_scoped_enum_v<Type>;
}  // namespace fuse

template <fuse::scoped_enum T>
[[nodiscard]] constexpr auto operator|(const T lhs, const T rhs) noexcept {
  return static_cast<T>(fuse::underlyingCast(lhs) | fuse::underlyingCast(rhs));
}

template <fuse::scoped_enum T>
[[nodiscard]] constexpr auto operator&(const T lhs, const T rhs) noexcept {
  return static_cast<T>(fuse::underlyingCast(lhs) & fuse::underlyingCast(rhs));
}

template <fuse::scoped_enum T>
[[nodiscard]] constexpr auto operator^(const T lhs, const T rhs) noexcept {
  return static_cast<T>(fuse::underlyingCast(lhs) ^ fuse::underlyingCast(rhs));
}

template <fuse::scoped_enum T>
[[nodiscard]] constexpr auto operator~(const T value) noexcept {
  return static_cast<T>(~fuse::underlyingCast(value));
}