#pragma once
#include <introspect/Config.hpp>

#include <fuse/TypeTraits.hpp>

namespace introspect
{
template <fuse::scalar T> class Vector4 {
public:
  constexpr Vector4() noexcept = default;

  constexpr Vector4(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}

  T x{}, y{}, z{}, w{};
};
}  // namespace introspect