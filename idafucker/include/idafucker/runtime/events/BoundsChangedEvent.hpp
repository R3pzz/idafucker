#pragma once
#include <memory>  // shared_ptr

#include <idafucker/Config.hpp>
#include <idafucker/math/Common.hpp>

namespace idafucker
{
class BoundsChangedEvent {
public:
  using Ref = std::shared_ptr<BoundsChangedEvent>;

  constexpr BoundsChangedEvent() noexcept = default;
  virtual constexpr ~BoundsChangedEvent() noexcept = default;
};

class SizeChangedEvent : public BoundsChangedEvent {
public:
  enum class Type {
    kDefault = 0,    //< A regular size change.
    kMinimized = 1,  //< The window was minimized.
    kMaximized = 2,  //< The window was maximized.
  };

  constexpr SizeChangedEvent(Type type, IntSize size) noexcept
      : type_{type}, size_{size} {}

  [[nodiscard]] constexpr Type type() const noexcept {
    return type_;
  }

  [[nodiscard]] constexpr IntSize size() const noexcept {
    return size_;
  }

private:
  Type type_{};
  IntSize size_{};
};
}  // namespace idafucker