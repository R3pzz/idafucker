#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/templates/IntrusiveHandle.hpp>
#include "Resource.hpp"

IDAFUCKER_NAMESPACE_BEGIN

template <typename T>
  requires std::is_base_of<Resource, T>::value
using ResourceRef = IntrusiveHandle<Resource>;

IDAFUCKER_NAMESPACE_END