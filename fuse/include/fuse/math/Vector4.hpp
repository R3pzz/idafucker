#pragma once
#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse
{

template <concepts::arithmetic Base>
class Vector4 {
public:
  constexpr Vector4() noexcept = default;

  constexpr Vector4(Base x, Base y, Base z, Base w) noexcept
      : x{std::move(x)}, y{std::move(y)}, z{std::move(z)}, w{std::move(w)} {}

  Base x{}, y{}, z{}, w{};
};

}  // namespace fuse