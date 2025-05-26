#pragma once
#include <idafucker/Config.hpp>
#include <idafucker/math/Common.hpp>

namespace idafucker
{
struct WindowOptions {
  enum class Type {
    Normal,
    Popup,
  };

  std::wstring title{};
  IntSize size{};
  bool fixedSize{false};
  Type type{Type::Normal};
};
}  // namespace idafucker