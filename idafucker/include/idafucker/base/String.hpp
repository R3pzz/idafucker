#pragma once
#include <idafucker/CoreDefines.hpp>

#include "TypeTraits.hpp"

IDAFUCKER_NAMESPACE_BEGIN

// std::basic_string/std::basic_string_view -like types.
template <typename T, typename Char>
concept string_like =
    is_any_of_v<T, std::basic_string<Char>, std::basic_string_view<Char>>;

struct ToWide final {
 private:
  [[nodiscard]] std::wstring impl(
      const char* data, const std::size_t size) const;

 public:
  constexpr ToWide() noexcept = default;

  template <string_like<char> String>
  [[nodiscard]] std::wstring operator()(const String& value) const
  {
    return impl(value.data(), value.size());
  }
};

struct ToNarrow final {
 private:
  [[nodiscard]] std::string impl(
      const wchar_t* data, const std::size_t size) const;

 public:
  constexpr ToNarrow() noexcept = default;

  template <string_like<wchar_t> String>
  [[nodiscard]] std::string operator()(const String& value) const
  {
    return impl(value.data(), value.size());
  }

  [[nodiscard]] std::string operator()(const wchar_t *value) const
  {
    std::size_t size{};
    for (; value[size] != '\0'; ++size)
      ;

    return impl(value, size);
  }
};

[[nodiscard]] auto widen(const auto& value)
{
  return ToWide{}(value);
}

[[nodiscard]] auto narrow(const auto& value)
{
  return ToNarrow{}(value);
}

IDAFUCKER_NAMESPACE_END