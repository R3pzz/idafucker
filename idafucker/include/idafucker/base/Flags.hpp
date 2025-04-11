#pragma once
#include <idafucker/CoreDefines.hpp>

#include "Bitmask.hpp"

IDAFUCKER_NAMESPACE_BEGIN

template <scoped_enum Enum> class Flags {
 public:
  constexpr Flags() noexcept = default;

  constexpr Flags(const Enum value) noexcept : value_{value} {}

  constexpr Flags(const std::underlying_type_t<Enum> value) noexcept
      : value_{static_cast<Enum>(value)}
  {
  }

  [[nodiscard]] constexpr bool operator==(const Enum rhs) const noexcept
  {
    return value_ == rhs;
  }

  constexpr auto& operator|=(const Enum rhs) noexcept
  {
    value_ = (value_ | rhs);
    return *this;
  }

  constexpr auto& operator&=(const Enum rhs) noexcept
  {
    value_ = (value_ & rhs);
    return *this;
  }

  [[nodiscard]] constexpr Enum operator|(const Enum rhs) const noexcept
  {
    return value_ | rhs;
  }

  [[nodiscard]] constexpr Enum operator&(const Enum rhs) const noexcept
  {
    return value_ & rhs;
  }

  [[nodiscard]] constexpr operator Enum() const noexcept
  {
    return value_;
  }

  [[nodiscard]] constexpr bool contains(const Enum flag) const noexcept
  {
    return (value_ & flag) != Enum{};
  }

  constexpr void add(const Enum flag) noexcept
  {
    value_ = (value_ | flag);
  }

  constexpr void remove(const Enum flag) noexcept
  {
    value_ = (value_ & ~flag);
  }

  [[nodiscard]] constexpr Enum get() const noexcept
  {
    return value_;
  }

 private:
  Enum value_{};
};

IDAFUCKER_NAMESPACE_END