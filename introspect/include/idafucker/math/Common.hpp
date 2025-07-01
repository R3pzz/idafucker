#pragma once
#include <introspect/Config.hpp>

#include "Rectangle.hpp"
#include "Vector2.hpp"

namespace introspect
{
// Floating point coordinate system trivial types.
using FloatSize = Vector2<float>;

// Integral coordinate system trivial types.
using IntSize = Vector2<int>;
using IntPos = Vector2<int>;
}  // namespace introspect