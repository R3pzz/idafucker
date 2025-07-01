#pragma once
#include <memory>  // shared_ptr

#include <introspect/Config.hpp>
#include <introspect/math/Common.hpp>

namespace introspect
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
    Default = 0,    //< A regular size change.
    Minimized = 1,  //< The window was minimized.
    Maximized = 2,  //< The window was maximized.
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
}  // namespace introspect