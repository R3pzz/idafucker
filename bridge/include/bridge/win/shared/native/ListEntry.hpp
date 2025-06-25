#pragma once
#include <fuse/Memory.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win
{

template <typename Address> struct ListEntry {
  static constexpr auto k_flink{0x0u}; // `Address`.
  static constexpr auto k_blink{k_flink + sizeof(Address)}; // `Address`.
};

} // namespace bridge::win