#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/Message.hpp>

IDAFUCKER_NAMESPACE_BEGIN

// `CloseMessage` is posted when a window gets closed, either by the user
// pressing `X` button or by the application calling `window.close()`.
class CloseMessage : public Message {
 public:
  constexpr CloseMessage() noexcept = default;

  [[nodiscard]] constexpr Type type() const noexcept override
  {
    return Type::Close;
  }
};

IDAFUCKER_NAMESPACE_END