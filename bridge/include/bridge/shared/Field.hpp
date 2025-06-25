#pragma once
#include <cstddef>

namespace bridge
{

template <typename Type, std::size_t Offset> struct Field {
  using FieldType = Type;

  static constexpr auto k_offset{Offset};

  constexpr Field() noexcept = default;
};

}  // namespace bridge