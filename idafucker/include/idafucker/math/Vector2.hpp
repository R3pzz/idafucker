#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/TypeTraits.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <scalar T> class Vector2 {
public:
  constexpr Vector2() noexcept = default;

  constexpr Vector2(T&& x, T&& y) noexcept
      : x{std::forward<T>(x)}, y{std::forward<T>(y)}
  {
  }

  T x{}, y{};
};

IDAFUCKER_NAMESPACE_END