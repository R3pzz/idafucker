#pragma once
#include <functional>  // function
#include <vector>      // vector

#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse
{
template <typename Type>
concept slot_like = std::is_function_v<Type>;

template <slot_like Signature> class Signal {
public:
  using Slot = std::function<Signature>;

  constexpr Signal() noexcept = default;

  // An alias operator for `connect()` function
  constexpr std::size_t operator<<(Slot &&slot) noexcept {
    return connect(std::forward<Slot>(slot));
  }

  // An alias operator for `emit()` function
  constexpr void operator()(auto &&...args) const noexcept {
    return emit(std::forward<decltype(args)>(args)...);
  }

  constexpr std::size_t connect(Slot &&slot) noexcept {
    slots.push_back(std::forward<Slot>(slot));
    return slots.size() - 1;
  }

  constexpr void remove(const std::size_t id) noexcept {
    slots.erase(std::begin(slots) + id);
  }

  constexpr void clear() noexcept {
    slots.clear();
  }

  void emit(auto &&...args) const noexcept {
    for (auto &&slot : slots)
      slot(std::forward<decltype(args)>(args)...);
  }

private:
  std::vector<Slot> slots{};
};
}  // namespace fuse