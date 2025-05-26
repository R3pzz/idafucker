#pragma once
#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <idafucker/Config.hpp>
#include <idafucker/math/Common.hpp>
#include <idafucker/runtime/events/BoundsChangedEvent.hpp>

namespace idafucker::messages
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

}  // namespace idafucker