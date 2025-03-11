#pragma once
#include <hyperui/CoreDefines.hpp>
#include <hyperui/events/EventRef.hpp>
#include <hyperui/style/StyleRef.hpp>
#include <idafucker/base/math/Common.hpp>
#include "InputState.hpp"

#include <functional>   // function
#include <memory>   // shared_ptr
#include <optional> // optional
#include <vector>   // vector

HYPERUI_NAMESPACE_BEGIN

class BaseWidget {
public:
  using Ref = std::shared_ptr<BaseWidget>;

  using UniqueId = std::size_t;

  constexpr BaseWidget() noexcept = default;
  explicit BaseWidget(const StyleRef &styleRef) noexcept : _styleRef{styleRef} {}
  
  // Presentation support
  virtual void render() = 0;
  // Event pilepline support
  virtual void onEvent(EventRef event) = 0;

  // Field getters
  [[nodiscard]] constexpr const auto &dimensions() const noexcept { return _dimensions; }

  // Layout support
  // This function may only get called in two situations:
  // - When a parent container class is laying out the widgets, OR
  // - When a user sets a custom extent/position for a standalone widget
  constexpr void setDimensions(const idafucker::Rectangle<float> &dimensions) noexcept { _dimensions = dimensions; }

  // State managing
  [[nodiscard]] constexpr auto inputState() const noexcept { return _state; }
  [[nodiscard]] constexpr bool inputEnabled() const noexcept { return _state == InputState::Enabled; }
  [[nodiscard]] constexpr bool inputDisabled() const noexcept { return _state == InputState::Disabled; }

private:
  StyleRef _styleRef{};

  // Rendering essentials
  idafucker::Rectangle<float> _dimensions{};

  // Input handling
  InputState _state{};

  // Optimization - keep track of 'dirtiness' of the widget so that
  // we don't re-render its texture when nothing has been changed
  bool _dirty{};
};

HYPERUI_NAMESPACE_END