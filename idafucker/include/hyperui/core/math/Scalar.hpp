#pragma once
#include <CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace hyperui
{

template <typename T>
concept scalar = std::is_scalar<T>::value;

} // namespace hyperui

IDAFUCKER_NAMESPACE_END