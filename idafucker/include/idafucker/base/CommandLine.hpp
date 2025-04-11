#pragma once
#include <ranges>  // find

#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

// Command line parser.
template <typename Character, Character Delimiter> class CommandLine {
 public:
  using View = std::basic_string_view<Character>;

  constexpr CommandLine(Character *data) noexcept
      : data_{tokenize(std::string_view{data})}
  {
  }

  constexpr CommandLine(View data) noexcept
    : data_{tokenize(data)}
  {
  }

  [[nodiscard]] constexpr bool hasFlag(
      const std::basic_string<Character> &name) const noexcept
  {
    return std::ranges::find(data_, name.c_str()) != std::end(data_);
  }

  [[nodiscard]] constexpr auto find(const std::basic_string<Character> &name)
      const noexcept -> std::basic_string_view<Character>
  {
    const auto it = std::ranges::find(data_, name.c_str());
    if (it == std::end(data_)) {
      return {};
    }

    const auto &next = *std::next(it);
    if (next.starts_with("-")) {
      return {};
    }

    return next;
  }

 private:
  [[nodiscard]] static constexpr auto tokenize(View data) noexcept
      -> std::vector<View>
  {
    std::vector<View> tokens{};
    std::size_t tokenStart{}, tokenEnd{};

    while ((tokenEnd = data.find(Delimiter, tokenStart)) != std::string::npos) {
      tokens.push_back(data.substr(tokenStart, tokenEnd - tokenStart));
      tokenStart = tokenEnd + 1u;
    }

    tokens.push_back(data.substr(tokenStart));
    return tokens;
  }

  std::vector<View> data_{};
};

template <typename Character, Character Delimiter>
CommandLine(std::string_view) -> CommandLine<char, ' '>;

template <typename Character, Character Delimiter>
CommandLine(std::wstring_view) -> CommandLine<wchar_t, L' '>;

#if defined(IDAFUCKER_PLATFORM_WIN32)
template <typename Character, Character Delimiter>
CommandLine(LPSTR) -> CommandLine<char, ' '>;

template <typename Character, Character Delimiter>
CommandLine(LPWSTR) -> CommandLine<wchar_t, L' '>;
#endif // defined(IDAFUCKER_PLATFORM_WIN32)

IDAFUCKER_NAMESPACE_END