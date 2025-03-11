#pragma once
#include <hyperui/CoreDefines.hpp>
#include "EventType.hpp"

#include <memory> // shared_ptr

HYPERUI_NAMESPACE_BEGIN

class Event {
public:
  virtual ~Event() noexcept = default;

  [[nodiscard]] virtual EventType type() const noexcept = 0;
};

HYPERUI_NAMESPACE_END