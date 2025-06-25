#pragma once
#include <fuse/Config.hpp>

namespace fuse
{

template <typename Address>
class Pointer {
  static_assert(std::is_integral_v<Address> && std::is_unsigned_v<Address>,
                "`Address` can only be an unsigned integer.");

public:
  constexpr Pointer() noexcept = default;

  constexpr Pointer(Address value) noexcept : value_{std::move(value)} {}

  constexpr Pointer(void *value) noexcept
      : value_{std::move(reinterpret_cast<std::uintptr_t>(value))} {}

  // We can treat is as a raw address.
  [[nodiscard]] constexpr operator Address() const noexcept {
    return value_;
  }

  // We can treat it as a pointer to any type.
  template <typename Type>
  [[nodiscard]] constexpr operator Type *() const noexcept {
    return reinterpret_cast<Type *>(value_);
  }

  // Get the pointer value.
  [[nodiscard]] constexpr auto value() const noexcept -> Address {
    return value_;
  }

  // Manual casting.
  template <typename Type>
    requires (std::is_pointer_v<Type>)
  [[nodiscard]] constexpr auto as() const noexcept -> Type {
    return reinterpret_cast<Type>(value_);
  }

protected:
  Address value_{};
};

template <typename Address, typename Type>
class BoundPointer {
  static_assert(std::is_integral_v<Address> && std::is_unsigned_v<Address>,
                "`Address` can only be an unsigned integer.");

public:
  constexpr BoundPointer() noexcept = default;

  constexpr BoundPointer(Address value) noexcept : value_{std::move(value)} {}

  constexpr BoundPointer(void *value) noexcept
      : value_{std::move(reinterpret_cast<std::uintptr_t>(value))} {}

  // We can dereference it.
  [[nodiscard]] constexpr auto operator->() const noexcept -> Type * {
    return reinterpret_cast<Type *>(value_);
  }

  // We can treat is as a raw address.
  [[nodiscard]] constexpr operator Address() const noexcept {
    return value_;
  }

  // We can treat it as a pointer of the bound type.
  [[nodiscard]] constexpr operator Type *() const noexcept {
    return reinterpret_cast<Type *>(value_);
  }

  // Get the pointer value.
  [[nodiscard]] constexpr auto value() const noexcept -> Address {
    return value_;
  }

  // Pointer retrieval.
  template <typename Type>
    requires (std::is_pointer_v<Type>)
  [[nodiscard]] constexpr auto get() const noexcept -> Type {
    return reinterpret_cast<Type>(value_);
  }

protected:
  Address value_{};
};

}  // namespace fuse