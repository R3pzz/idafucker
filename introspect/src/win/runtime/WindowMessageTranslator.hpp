#pragma once
#include <introspect/Config.hpp>
#include <introspect/math/Common.hpp>
#include <introspect/runtime/events/BoundsChangedEvent.hpp>
#include <fuse/Platform.hpp>

namespace introspect::messages
{

class SizeMessage {
public:
  // A helper to extract the size argument from `wparam` and `lparam`.
  [[nodiscard]] static constexpr IntSize translateSize(LPARAM lparam) noexcept {
    return IntSize{LOWORD(lparam), HIWORD(lparam)};
  }

  // A helper to extract the way the window size was changed.
  [[nodiscard]] static constexpr SizeChangedEvent::Type translateType(
      WPARAM wparam) noexcept {
    return static_cast<SizeChangedEvent::Type>(wparam);
  }
};

}  // namespace introspect