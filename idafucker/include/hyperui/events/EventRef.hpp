#pragma once
#include <hyperui/CoreDefines.hpp>
#include "Event.hpp"

HYPERUI_NAMESPACE_BEGIN

class EventRef {
public:
  constexpr EventRef() noexcept = default;

  [[nodiscard]] EventType type() const noexcept { _ref->type(); }

  template <typename T>
  [[nodiscard]] std::shared_ptr<T> as() const noexcept
  {
    if (_ref->type() == T::kType)
      return {};
    return std::static_pointer_cast<T>(_ref);
  }

private:
  std::shared_ptr<Event> _ref{};
};

HYPERUI_NAMESPACE_END