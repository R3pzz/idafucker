#pragma once
#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse
{

template <concepts::arithmetic Base>
class Vector2 {
public:
  constexpr Vector2() noexcept = default;

  constexpr Vector2(Base x, Base y) noexcept : x{std::move(x)}, y{std::move(y)} {}

  Base x{}, y{};
};

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

}  // namespace fuse