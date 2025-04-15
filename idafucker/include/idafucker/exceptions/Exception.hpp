#pragma once
#include <format>           // format
#include <source_location>  // source_location

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/TypeTraits.hpp>

//
// `ExceptionStream` is essentially a message builder for a final exception.
//
// You can have different exception streams with different modifiers.
//

#include <iostream>

IDAFUCKER_NAMESPACE_BEGIN

namespace decor
{
struct Hex final {
  constexpr Hex() noexcept = default;

  [[nodiscard]] static std::string format(const void* token)
  {
    return std::format("0x{:X}", reinterpret_cast<std::uintptr_t>(token));
  }
};

constexpr Hex hex{};
}  // namespace decor

template <typename T>
concept formatter_like = std::is_same_v<
    typename std::invoke_result<decltype(&T::format), const void*>::type,
    std::string>;

class Exception : public std::exception {
  using FormatFunc = std::string(const void*);

  [[nodiscard]] static std::string formatSourceLocation(
      const std::source_location& src_loc)
  {
    return std::format(
        "[{}:{}] `{}`: ", src_loc.file_name(), src_loc.line(),
        src_loc.function_name());
  }

 public:
  explicit Exception(
      const std::source_location src_loc =
          std::source_location::current()) noexcept
      : message_{formatSourceLocation(src_loc)}, nextFormatter_{}
  {
  }

  explicit Exception(
      const char* message, const std::source_location src_loc =
                               std::source_location::current()) noexcept
      : message_{formatSourceLocation(src_loc) + message}, nextFormatter_{}
  {
  }

  explicit Exception(
      std::string message, const std::source_location src_loc =
                               std::source_location::current()) noexcept
      : message_{formatSourceLocation(src_loc) + std::move(message)},
        nextFormatter_{}
  {
  }

  template <formatter_like T> constexpr auto& operator<<(T formatter) noexcept
  {
    nextFormatter_ = &T::format;
    return *this;
  }

  constexpr auto& operator<<(const char* message) noexcept
  {
    message_ += message;
    return *this;
  }

  constexpr auto& operator<<(const std::string& message) noexcept
  {
    message_ += message;
    return *this;
  }

  auto& operator<<(const int value) noexcept
  {
    if (nextFormatter_ != nullptr) {
      message_ += nextFormatter_(reinterpret_cast<const void*>(value));
      nextFormatter_ = nullptr;
    } else {
      message_ += std::format("{}", value);
    }

    return *this;
  }

  [[nodiscard]] virtual const char* what() const noexcept override
  {
    return message_.c_str();
  }

 protected:
  std::string message_;
  FormatFunc* nextFormatter_;
};

IDAFUCKER_NAMESPACE_END