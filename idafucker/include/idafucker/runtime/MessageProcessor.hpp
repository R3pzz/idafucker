#pragma once
#include <functional>  // function
#include <memory>      // shared_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/Signal.hpp>

#include "Message.hpp"

IDAFUCKER_NAMESPACE_BEGIN

class MessageProcessor {
 public:
  virtual ~MessageProcessor() noexcept = default;

  // Callbacks
  Signal<void(Message::Ref)> onMessage{};
  Signal<void()> onMessageLoop{};
};

IDAFUCKER_NAMESPACE_END