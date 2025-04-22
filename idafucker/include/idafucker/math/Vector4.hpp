#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/TypeTraits.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <scalar T> class Vector4 {
public:
  constexpr Vector4() noexcept = default;

  constexpr Vector4(T x, T y, T z, T w) noexcept : x{x}, y{y}, z{z}, w{w} {}

  T x{}, y{}, z{}, w{};
};

IDAFUCKER_NAMESPACE_END