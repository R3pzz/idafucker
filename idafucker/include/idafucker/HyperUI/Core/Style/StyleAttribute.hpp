#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/HyperUI/Core/Math/Rectangle.hpp>
#include <idafucker/HyperUI/Core/Math/Vector2.hpp>

#include <any> // any
#include <type_traits> // is_same

IDAFUCKER_NAMESPACE_BEGIN

namespace hyperui
{

template <typename T>
concept attribute = std::_Is_any_of_v<T, bool, int, float, Vector2<int>, Vector2<float>, Rectangle<float>, std::string>;

enum class StyleAttributeType : int {
  Bool,
  Int,
  Float,
  Vector2i,
  Vector2f,
  Rectangle,
  String
};

// Describes a given property of a style
class StyleAttribute {
public:
  constexpr StyleAttribute() noexcept = default;

  [[nodiscard]] constexpr StyleAttributeType type() const noexcept {
    const auto &typeInfo = _value.type().;
    
  }

  [[nodiscard]] constexpr const std::string &name() const noexcept { return _name; }

private:
  std::string _name{};
  std::any _value{};
};
} // namespace hyperui

IDAFUCKER_NAMESPACE_END