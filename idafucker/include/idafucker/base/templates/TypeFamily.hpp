#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace detail
{
struct TypeFamilyIndexGenerator {
  inline static std::size_t index{0u};

  [[nodiscard]] static constexpr auto next() noexcept { return ++index; }
};
} // namespace detail

template <typename T>
struct TypeFamily {
  [[nodiscard]] static auto value() noexcept
  {
    static const auto result = detail::TypeFamilyIndexGenerator::next();
    return result;
  }
};

using TypeFamilyId = std::size_t;

IDAFUCKER_NAMESPACE_END