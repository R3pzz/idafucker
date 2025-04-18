#pragma once
#include <idafucker/CoreDefines.hpp>

#include "TypeTraits.hpp"

IDAFUCKER_NAMESPACE_BEGIN

// std::basic_string/std::basic_string_view -like types.
template <typename T, typename Char>
concept string_like =
    is_any_of_v<T, std::basic_string<Char>, std::basic_string_view<Char>>;

struct ToUnicode final {
 private:
  [[nodiscard]] std::wstring impl(
      const char* data, const std::size_t size) const;

 public:
  constexpr ToUnicode() noexcept = default;

  template <string_like<char> String>
  [[nodiscard]] std::wstring operator()(const String& value) const
  {
    return impl(value.data(), value.size());
  }
};

struct ToUtf8 final {
 private:
  [[nodiscard]] std::string impl(
      const wchar_t* data, const std::size_t size) const;

 public:
  constexpr ToUtf8() noexcept = default;

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

[[nodiscard]] auto convertToUnicode(const auto& value)
{
  return ToUnicode{}(value);
}

[[nodiscard]] auto convertToUtf8(const auto& value)
{
  return ToUtf8{}(value);
}

IDAFUCKER_NAMESPACE_END