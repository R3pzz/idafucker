#pragma once
#include <idafucker/Config.hpp>

#include <fuse/TypeTraits.hpp>

namespace idafucker
{
template <fuse::scalar T> class Vector4 {
public:
  constexpr Vector4() noexcept = default;

  constexpr Vector4(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}

  T x{}, y{}, z{}, w{};
};
}  // namespace idafucker