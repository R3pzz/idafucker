#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace win::detail {
enum WindowClassStyle : DWORD {
  RedrawOnVerticalSizeChanged = CS_VREDRAW,
  RedrawOnHorizontalSizeChanged = CS_HREDRAW,
  RedrawOnSizeChanged =
      RedrawOnVerticalSizeChanged | RedrawOnHorizontalSizeChanged
};
}  // namespace win::detail

IDAFUCKER_NAMESPACE_END