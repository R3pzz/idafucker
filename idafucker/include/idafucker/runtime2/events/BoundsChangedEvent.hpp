#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/math/Common.hpp>

#include <memory> // shared_ptr

IDAFUCKER_NAMESPACE_BEGIN

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

  constexpr SizeChangedEvent(Type type, Size size) noexcept
      : type_{type}, size_{size}
  {
  }

  [[nodiscard]] constexpr Type type() const noexcept
  {
    return type_;
  }

  [[nodiscard]] constexpr Size size() const noexcept
  {
    return size_;
  }

 private:
  Type type_{};
  Size size_{};
};

IDAFUCKER_NAMESPACE_END