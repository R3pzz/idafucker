#pragma once
#include <filesystem>  // path
#include <ranges>      // find
#include <vector>      // vector

#include <fuse/Config.hpp>

namespace fuse
{
namespace detail
{
template <typename>
struct select_argument_identifier {};

template <>
struct select_argument_identifier<char> {
  static constexpr auto value = '-';
};

template <>
struct select_argument_identifier<wchar_t> {
  static constexpr auto value = L'-';
};

template <typename>
struct select_delimiter {};

template <>
struct select_delimiter<char> {
  static constexpr auto value = ' ';
};

template <>
struct select_delimiter<wchar_t> {
  static constexpr auto value = L' ';
};
}  // namespace detail

// Command line argument.
template <typename Character>
class Argument {
public:
  using View = std::basic_string_view<Character>;

  constexpr Argument() noexcept = default;

  constexpr Argument(View name) noexcept : name_{name} {}

  constexpr Argument(View name, View value) noexcept : name_{name}, value_{value} {}

  [[nodiscard]] operator std::filesystem::path() const noexcept {
    return std::filesystem::path{value_};
  }

  // Flag properties
  [[nodiscard]] constexpr bool isSet() const noexcept {
    return !name_.empty();
  }

  // Parameter properties
  [[nodiscard]] constexpr bool hasValue() const noexcept {
    return !value_.empty();
  }

  [[nodiscard]] constexpr View value() const noexcept {
    return value_;
  }

  [[nodiscard]] constexpr View valueOr(View &&backup) const noexcept {
    return value_.empty() ? std::move(backup) : value_;
  }

  [[nodiscard]] constexpr const View &valueOr(const View &backup) const noexcept {
    return value_.empty() ? backup : value_;
  }

  // Cast-to
  [[nodiscard]] constexpr std::filesystem::path toPath() const noexcept {
    return {value_};
  }

private:
  View name_{};
  View value_{};
};

// Command line parser.
template <typename Character>
class CommandLine {
public:
  using View = std::basic_string_view<Character>;

  static constexpr auto kDelimiter = detail::select_delimiter<Character>::value;
  static constexpr auto
      kArgumentIdentifier = detail::select_argument_identifier<Character>::value;

  constexpr CommandLine(Character *data) noexcept : data_{tokenize(data)} {}

  constexpr CommandLine(View data) noexcept : data_{tokenize(data)} {}

  [[nodiscard]] constexpr auto find(
      const std::basic_string<Character> &name) const noexcept -> Argument<Character> {
    const auto it = std::ranges::find(data_, name.c_str());
    if (it == std::end(data_))
      return {};

    const auto next = std::next(it);
    if (next == std::end(data_) || next->starts_with(kArgumentIdentifier))
      return {*it};

    return {*it, *next};
  }

private:
  [[nodiscard]] static constexpr auto tokenize(View data) noexcept -> std::vector<View> {
    std::vector<View> tokens{};
    std::size_t tokenStart{}, tokenEnd{};

    while ((tokenEnd = data.find(kDelimiter, tokenStart)) != std::string::npos) {
      tokens.push_back(data.substr(tokenStart, tokenEnd - tokenStart));
      tokenStart = tokenEnd + 1u;
    }

    tokens.push_back(data.substr(tokenStart));
    return tokens;
  }

  std::vector<View> data_{};
};

#if 0
template <typename Character>
CommandLine(std::string_view) -> CommandLine<char>;
template <typename Character>
CommandLine(std::wstring_view) -> CommandLine<wchar_t>;
#endif

// Retrieve a command line that was passed to the system to create the current
// process.
[[nodiscard]] CommandLine<wchar_t> systemCommandLine() noexcept;
}  // namespace fuse