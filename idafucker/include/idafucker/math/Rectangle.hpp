#pragma once
#include <idafucker/CoreDefines.hpp>

#include "Vector2.hpp"

IDAFUCKER_NAMESPACE_BEGIN

template <scalar T> class Rectangle {
public:
  constexpr Rectangle() noexcept = default;

  constexpr Rectangle(T left, T top, T right, T bottom) noexcept
      : left{left}, top{top}, right{right}, bottom{bottom}
  {
  }

  constexpr Rectangle(const Vector2<T>& mins, const Vector2<T>& maxs) noexcept
  {
    fromBounds(mins, maxs);
  }

  constexpr auto& fromBounds(
      const Vector2<T>& mins, const Vector2<T>& maxs) noexcept
  {
    left = mins.x;
    top = mins.y;
    right = maxs.x;
    bottom = maxs.y;

    return *this;
  }

  [[nodiscard]] constexpr auto extent() const noexcept -> Vector2<float>
  {
    return {right - left, bottom - top};
  }

  T left{}, top{}, right{}, bottom{};
};

IDAFUCKER_NAMESPACE_END