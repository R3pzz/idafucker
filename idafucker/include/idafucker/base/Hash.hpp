#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

struct Fnv1aSpecifications {
  static constexpr auto basis =
      IDAFUCKER_EACH_ARCH(0x811c9dc5u, 0xcbf29ce484222325ull);
  static constexpr auto prime =
      IDAFUCKER_EACH_ARCH(0x01000193u, 0x00000100000001b3ull);
};

template <typename T> struct Hash;

template <typename Base> struct Hash<Base[]> {
  constexpr Hash() noexcept = default;

  [[nodiscard]] constexpr std::size_t operator()(
      const Base data[], std::size_t length) const noexcept
  {
    auto hash{Fnv1aSpecifications::basis};
    for (std::size_t i{}; i < length; ++i) {
      hash ^= static_cast<std::size_t>(data[i]);
      hash *= Fnv1aSpecifications::prime;
    }

    return hash;
  }
};

template <typename Char> struct Hash<std::basic_string_view<Char>> {
  constexpr Hash() noexcept = default;

  [[nodiscard]] constexpr auto operator()(std::basic_string_view<Char> view)
      const noexcept -> decltype(Hash<Char[]>{}(view.data(), view.size()))
  {
    return Hash<Char[]>{}(view.data(), view.size());
  }
};

template <typename Char> struct Hash<std::basic_string<Char>> {
  constexpr Hash() noexcept = default;

  [[nodiscard]] constexpr auto operator()(std::basic_string<Char> view)
      const noexcept -> decltype(Hash<Char[]>{}(view.data(), view.size()))
  {
    return Hash<Char[]>{}(view.data(), view.size());
  }
};

IDAFUCKER_NAMESPACE_END