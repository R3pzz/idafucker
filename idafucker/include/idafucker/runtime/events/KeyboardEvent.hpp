#pragma once
#include <memory>  // shared_ptr

#include <idafucker/Config.hpp>
#include <idafucker/math/Common.hpp>

namespace idafucker
{
class KeyboardEvent {
public:
  enum class Type {
    kKeyInput,        //< Indicates that a certain key on the keyboard was
                      // pressed/released
    kCharacterInput,  //< Indicates that a certain character was typed in
  };

  enum class State {
    kPressed,
    kReleased,
  };

  using Ref = std::shared_ptr<KeyboardEvent>;

  constexpr KeyboardEvent(std::size_t virtualCode, State state) noexcept
      : type_{Type::kKeyInput}, virtualKeyCode_{virtualCode}, state_{state} {}

  [[nodiscard]] constexpr Type type() const noexcept {
    return type_;
  }

  [[nodiscard]] constexpr State state() const noexcept {
    return state_;
  }

  [[nodiscard]] constexpr std::size_t virtualKeyCode() const noexcept {
    return virtualKeyCode_;
  }

private:
  Type type_{};
  State state_{};

  union {
    std::size_t virtualKeyCode_{};
    char characterCode_;
  };

  bool isAltPressed_{};
  bool wasPressedBefore_{};
  std::uint16_t repeatCount_{};
};
}  // namespace idafucker