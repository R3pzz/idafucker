#pragma once
#include <fuse/Config.hpp>
#include <fuse/math/Vector2.hpp>

namespace fuse
{

template <concepts::arithmetic Base>
class Rectangle {
public:
  constexpr Rectangle() noexcept = default;

  constexpr Rectangle(Base left, Base top, Base right, Base bottom) noexcept
      : left{std::move(left)},
        top{std::move(top)},
        right{std::move(right)},
        bottom{std::move(bottom)} {}

  constexpr Rectangle(const Vector2<Base>& mins, const Vector2<Base>& maxs) noexcept
      : left{mins.x}, top{mins.y}, right{maxs.x}, bottom{maxs.y} {}

  [[nodiscard]] constexpr auto extent() const noexcept -> Vector2<float> {
    return {right - left, bottom - top};
  }

  Base left{}, top{}, right{}, bottom{};
};

using IntRectangle = Rectangle<int>;
using FloatRectangle = Rectangle<float>;

}  // namespace fuse