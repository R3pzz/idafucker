#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <typename Enum>
  requires(std::is_enum<Enum>::value)
class BitFlags {
public:
  using EnumBase = typename std::underlying_type<Enum>::type;

  constexpr BitFlags() noexcept = default;
  constexpr BitFlags(Enum value) noexcept : _value{static_cast<EnumBase>(value)} {}
  constexpr BitFlags(EnumBase value) noexcept : _value{value} {}

  [[nodiscard]] constexpr bool operator==(Enum rhs) const noexcept { return _value == static_cast<EnumBase>(rhs); }

  constexpr auto &operator|=(Enum rhs) noexcept
  {
    _value |= static_cast<EnumBase>(rhs);
    return *this;
  }

  constexpr auto &operator&=(Enum rhs) noexcept
  {
    _value &= static_cast<EnumBase>(rhs);
    return *this;
  }

  [[nodiscard]] constexpr BitFlags operator|(Enum rhs) const noexcept { return {_value | static_cast<EnumBase>(rhs)}; }
  [[nodiscard]] constexpr BitFlags operator&(Enum rhs) const noexcept { return {_value & static_cast<EnumBase>(rhs)}; }

  [[nodiscard]] constexpr operator Enum() const noexcept { return static_cast<Enum>(_value); }

  [[nodiscard]] constexpr bool contains(Enum flag) const noexcept { return _value & static_cast<EnumBase>(flag); }

  constexpr void add(Enum flag) noexcept { _value |= static_cast<EnumBase>(flag); }
  constexpr void remove(Enum flag) noexcept { _value &= ~static_cast<EnumBase>(flag); }

  [[nodiscard]] constexpr auto get() const noexcept { return _value; }

private:
  EnumBase _value{};
};

IDAFUCKER_NAMESPACE_END