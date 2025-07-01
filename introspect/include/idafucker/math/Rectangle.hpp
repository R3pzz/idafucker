#pragma once
#include <introspect/Config.hpp>
#include <introspect/math/Vector2.hpp>

namespace introspect
{
template <fuse::scalar T> struct Rectangle {
  constexpr Rectangle() noexcept = default;

  constexpr Rectangle(T left, T top, T right, T bottom) noexcept
      : left{left}, top{top}, right{right}, bottom{bottom} {}

  constexpr Rectangle(const Vector2<T>& mins, const Vector2<T>& maxs) noexcept
      : left{mins.x}, top{mins.y}, right{maxs.x}, bottom{maxs.y} {}

  [[nodiscard]] constexpr auto extent() const noexcept -> Vector2<float> {
    return {right - left, bottom - top};
  }

  T left{}, top{}, right{}, bottom{};
};
}  // namespace introspect