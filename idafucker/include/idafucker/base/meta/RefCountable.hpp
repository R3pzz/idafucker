#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <typename T>
concept ref_countable = requires(T t)
{
  { t.onReferenceAdded() };
  { t.onReferenceRemoved() };
};

IDAFUCKER_NAMESPACE_END