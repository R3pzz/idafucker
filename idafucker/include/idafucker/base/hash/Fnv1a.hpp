#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

struct Fnv1aSpecifications {
  static constexpr auto kBasis = IDAFUCKER_EACH_ARCH(0x811c9dc5u, 0xcbf29ce484222325ull);
  static constexpr auto kPrime = IDAFUCKER_EACH_ARCH(0x01000193u, 0x00000100000001b3ull);
};

template <typename T>
struct Fnv1aHash;

template <typename Base>
struct Fnv1aHash<Base[]> {
  constexpr Fnv1aHash() noexcept = default;

  [[nodiscard]] constexpr auto operator()(const Base data[], std::size_t length) const noexcept
  {
    auto hash{Fnv1aSpecifications::kBasis};
    for (std::size_t i{}; i < length; ++i) {
      hash ^= static_cast<std::size_t>(data[i]);
      hash *= Fnv1aSpecifications::kPrime;
    }

    return hash;
  }
};

template <typename Char>
struct Fnv1aHash<std::basic_string_view<Char>> {
  constexpr Fnv1aHash() noexcept = default;

  [[nodiscard]] constexpr auto operator()(std::basic_string_view<Char> view) const noexcept
  {
    return Fnv1aHash<Char[]>{}(view.data(), view.size());
  }
};

template <typename Char>
struct Fnv1aHash<std::basic_string<Char>> {
  constexpr Fnv1aHash() noexcept = default;

  [[nodiscard]] constexpr auto operator()(std::basic_string<Char> view) const noexcept
  {
    return Fnv1aHash<Char[]>{}(view.data(), view.size());
  }
};

IDAFUCKER_NAMESPACE_END