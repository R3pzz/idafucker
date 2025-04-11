#pragma once
#include <memory>  // shared_ptr

#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

class Message {
 public:
  enum class Type {
    Close
  };

  using Ref = std::shared_ptr<Message>;

  constexpr Message() noexcept = default;

  [[nodiscard]] virtual constexpr Type type() const noexcept = 0;
};

IDAFUCKER_NAMESPACE_END