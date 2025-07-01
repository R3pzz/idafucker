#pragma once
#include <introspect/Config.hpp>

#include <fuse/TypeTraits.hpp>

namespace introspect
{
template <fuse::scalar Base> class Vector2 {
public:
  constexpr Vector2() noexcept = default;

  constexpr Vector2(Base&& x, Base&& y) noexcept
      : x{std::forward<Base>(x)}, y{std::forward<Base>(y)} {}

  Base x{}, y{};
};
}  // namespace introspect