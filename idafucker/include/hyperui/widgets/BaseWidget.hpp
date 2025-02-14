#pragma once
#include <CoreDefines.hpp>
#include <hyperui/core/math/Rectangle.hpp>
#include <hyperui/core/style/StyleGroup.hpp>

#include <functional>   // function
#include <memory>   // shared_ptr
#include <optional> // optional
#include <vector>   // vector

IDAFUCKER_NAMESPACE_BEGIN

namespace hyperui
{

class BaseWidget {
public:
  using Ref = std::shared_ptr<BaseWidget>;

  constexpr BaseWidget() noexcept = default;
  constexpr BaseWidget(StyleGroup style) noexcept : _style{style} {}

  [[nodiscard]] virtual bool isRoot() const noexcept { return false; }

  constexpr void addChildren(std::initializer_list<Ref> widgets) { _children.insert(_children.end(), widgets); }
  constexpr void removeChildren(std::initializer_list<Ref> widgets) {
    std::remove_if(_children.begin(), _children.end(), [&widgets](const Ref &e) -> bool {
      return std::find(widgets.begin(), widgets.end(), e) != widgets.end();
    });
  }

  // Used by the layout engine
  void recurse(const std::function<void(BaseWidget &)> &fn);
  void recurse(const std::function<void(const BaseWidget &)> &fn) const;

  [[nodiscard]] constexpr StyleGroup style() const noexcept { return _style; }

private:
  std::vector<Ref> _children{};

  // Styling support
  StyleGroup _style{};

  // Layout support
  Rectangle<float> _bounds{};
};

} // namespace hyperui

IDAFUCKER_NAMESPACE_END