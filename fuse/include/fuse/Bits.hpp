#pragma once
#include <initializer_list>  // initializer_list

#include <fuse/Config.hpp>
#include <fuse/Enum.hpp>

namespace fuse
{
class BitRange {
public:
  constexpr BitRange() noexcept = default;

  constexpr BitRange(std::uint8_t bit) noexcept : begin_{bit}, end_{bit} {}

  constexpr BitRange(std::uint8_t begin, std::uint8_t end) noexcept
      : begin_{begin}, end_{end} {}

  [[nodiscard]] constexpr std::uint8_t begin() const noexcept {
    return begin_;
  }

  [[nodiscard]] constexpr std::uint8_t end() const noexcept {
    return end_;
  }

  template <typename Base>
    requires std::is_unsigned_v<Base>
  [[nodiscard]] constexpr Base mask() const noexcept {
    const auto width = end_ - begin_ + 1u;
    const auto set = (static_cast<Base>(1) << width) - static_cast<Base>(1);
    return set << begin_;
  }

  // Mask out the value and shift it to the beginning.
  [[nodiscard]] constexpr auto fetchFrom(auto value) const noexcept
      -> decltype(value) {
    return (value & mask<decltype(value)>()) >> begin_;
  }

  // Shift the value to the correct position and ensure
  // that the value is trucated to fit the width.
  [[nodiscard]] constexpr auto storeTo(auto value) const noexcept
      -> decltype(value) {
    return (value << begin_) & mask<decltype(value)>();
  }

private:
  std::uint8_t begin_{};
  std::uint8_t end_{};
};

namespace detail
{
// The problem this class solves is that when we pass a scoped enum argument,
// for example, as a flag which we want to check, the compiler will give us
// an error since a scoped enum is not implicitly convertible to its underlying
// type. This class separates scoped enums from standard integer types and
// provides a way to forward the arguments while automatically unpacking the
// scoped enum into its underlying type.
template <typename T> struct bit_set_mask_adapter {
  using Base = typename std::conditional_t<
      std::is_enum_v<T>, std::underlying_type<T>, std::type_identity<T>>::type;

  [[nodiscard]] static constexpr auto forward(const auto &value) noexcept {
    return reinterpret_cast<const Base &>(value);
  }

  // Forward the same reference we passed in.
  [[nodiscard]] static constexpr auto forward(auto &&value) noexcept {
    return reinterpret_cast<Base &&>(value);
  }
};
}  // namespace detail

template <typename T>
  requires(std::is_unsigned_v<T> || std::is_enum_v<T>)
class Bits {
private:
  using Adapter = detail::bit_set_mask_adapter<T>;

public:
  using Base = typename Adapter::Base;

  constexpr Bits() noexcept = default;

  constexpr Bits(auto &&value) noexcept : bits_{Adapter::forward(value)} {}

  [[nodiscard]] constexpr bool operator==(const auto rhs) const noexcept {
    return bits_ == Adapter::forward(rhs);
  }

  constexpr Bits &operator|=(const auto rhs) noexcept {
    bits_ = (bits_ | Adapter::forward(rhs));
    return *this;
  }

  constexpr Bits &operator&=(const auto rhs) noexcept {
    bits_ = (bits_ & Adapter::forward(rhs));
    return *this;
  }

  [[nodiscard]] constexpr Base operator|(const auto rhs) const noexcept {
    return bits_ | Adapter::forward(rhs);
  }

  [[nodiscard]] constexpr Base operator&(const auto rhs) const noexcept {
    return bits_ & Adapter::forward(rhs);
  }

  [[nodiscard]] constexpr operator Base() const noexcept {
    return bits_;
  }

  [[nodiscard]] constexpr bool contains(const auto flag) const noexcept {
    return (bits_ & Adapter::forward(flag)) != Base{};
  }

  constexpr void add(const auto flag) noexcept {
    bits_ = (bits_ | Adapter::forward(flag));
  }

  constexpr void remove(const auto flag) noexcept {
    bits_ = (bits_ & ~Adapter::forward(flag));
  }

  [[nodiscard]] constexpr Base get() const noexcept {
    return bits_;
  }

  [[nodiscard]] constexpr Base read(BitRange range) const noexcept {
    return range.fetchFrom(bits_);
  }

private:
  Base bits_{};
};
}  // namespace fuse