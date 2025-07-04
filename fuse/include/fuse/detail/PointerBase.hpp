#pragma once
#include <fuse/Config.hpp>

namespace fuse::detail
{

template <typename Address>
class PointerBase {
  static_assert(std::is_integral_v<Address> && std::is_unsigned_v<Address>,
                "`Address` can only be an unsigned integer.");

public:
  constexpr PointerBase() noexcept = default;

  constexpr PointerBase(Address value) noexcept : value_{std::move(value)} {}

  constexpr PointerBase(void *value) noexcept
      : value_{std::move(reinterpret_cast<std::uintptr_t>(value))} {}

  // We can treat is as a raw address.
  [[nodiscard]] constexpr operator Address() const noexcept {
    return value_;
  }

  [[nodiscard]] constexpr auto get() const noexcept -> Address {
    return value_;
  }

protected:
  Address value_{};
};

}  // namespace fuse::detail