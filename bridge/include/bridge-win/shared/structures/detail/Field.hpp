#pragma once
#include <fuse/TypeTraits.hpp>

namespace bridge::win::structures::detail
{

template <typename Type, std::size_t Offset>
struct Field {
  static constexpr auto k_offset{Offset};

  constexpr Field() noexcept = default;

  [[nodiscard]] static constexpr auto on(
      fuse::concepts::represents_address auto address) noexcept -> decltype(address) {
    using Address = decltype(address);

    if constexpr (std::is_pointer_v<Address>) {
      return reinterpret_cast<Address>(reinterpret_cast<std::uintptr_t>(address) +
                                       k_offset);
    } else {
      return address + k_offset;
    }
  }
};

template <typename Type, std::size_t Offset>
[[nodiscard]] constexpr auto operator+(fuse::concepts::represents_address auto lhs,
                                       Field<Type, Offset> rhs) noexcept
    -> decltype(lhs) {
  return lhs + Field<Type, Offset>::k_offset;
}

}  // namespace bridge::win::structures::detail