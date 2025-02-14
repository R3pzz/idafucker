#pragma once
#include <CoreDefines.hpp>
#include "Scalar.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace hyperui {

template <scalar T>
class Vector2 {
public:
  constexpr Vector2() noexcept = default;

  T x{}, y{};
};

} // namespace hyperui

IDAFUCKER_NAMESPACE_END