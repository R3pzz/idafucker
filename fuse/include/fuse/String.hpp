#pragma once
#include <string>  // basic_string, basic_string_view

#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse
{
// std::basic_string/std::basic_string_view -like types.
template <typename T, typename Char>
concept std_string_like = is_any_of_v<T,
                                      std::basic_string<Char>,
                                      std::basic_string_view<Char>>;

struct ToUnicode final {
  constexpr ToUnicode() noexcept = default;

  template <std_string_like<char> String>
  [[nodiscard]] std::wstring operator()(const String& value) const {
    return impl(value.data(), value.size());
  }

private:
  [[nodiscard]] static std::wstring impl(const char* data, const std::size_t size);
};

struct ToUtf8 final {
  constexpr ToUtf8() noexcept = default;

  template <std_string_like<wchar_t> String>
  [[nodiscard]] std::string operator()(const String& value) const {
    return impl(value.data(), value.size());
  }

  [[nodiscard]] std::string operator()(const wchar_t* value) const {
    std::size_t size{};
    for (; value[size] != '\0'; ++size)
      ;

    return impl(value, size);
  }

private:
  [[nodiscard]] static std::string impl(const wchar_t* data, const std::size_t size);
};

[[nodiscard]] auto unicodeCast(const auto& value) {
  return ToUnicode{}(value);
}

[[nodiscard]] auto utf8Cast(const auto& value) {
  return ToUtf8{}(value);
}
}  // namespace fuse