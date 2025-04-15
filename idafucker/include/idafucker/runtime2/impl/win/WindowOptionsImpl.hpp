#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/math/Common.hpp>

#include "Types.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace impl::win {
struct WindowOptionsImpl {
  enum class Type {
    Normal,
    Popup,
  };

  ATOM atom{};
  String title{};
  Size size{};
  bool fixedSize{false};
  Type type{Type::Normal};
};
}  // namespace impl::win

IDAFUCKER_NAMESPACE_END