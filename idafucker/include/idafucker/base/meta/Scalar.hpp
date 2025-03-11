#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <typename T>
concept scalar = std::is_scalar<T>::value;

IDAFUCKER_NAMESPACE_END