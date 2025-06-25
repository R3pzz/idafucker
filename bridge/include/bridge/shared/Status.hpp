#pragma once
#include <expected>    // expected
#include <stacktrace>  // stacktrace
#include <string>      // string

#include <fuse/TypeTraits.hpp>

namespace bridge
{

struct Status {
public:
  enum class Code {
    Success,
    AccessDenied,
    BadAddress,
    BadProcess,
    BadHandle,
    InvalidArgument,
    InvalidSignature,
    NotFound,
    NotImplemented,
    SymbolNotFound,
    SystemSpecific,
    Protection,
    Unknown,
  };

  // Construct a `Status` object with a `Success` code and no payloads.
  constexpr Status() noexcept = default;

  // Construct a `Status` object with the `code` code and a payload list.
  constexpr Status(Code code,
                   std::stacktrace trace,
                   auto &&...payloads) noexcept
      : code_{std::move(code)},
        trace_{std::move(trace)},
        payloads_{std::forward<decltype(payloads)>(payloads)...} {}

  [[nodiscard]] constexpr bool operator!() const noexcept {
    return code_ != Code::Success;
  }

  template <typename Type>
  [[nodiscard]] constexpr operator std::expected<Type, Status>()
      const & noexcept {
    return std::unexpected{*this};
  }

  template <typename Type>
  [[nodiscard]] constexpr operator std::expected<Type, Status>()
      const && noexcept {
    return std::unexpected{std::move(*this)};
  }

  [[nodiscard]] constexpr bool success() const noexcept {
    return code_ == Code::Success;
  }

  [[nodiscard]] constexpr Code code() const noexcept {
    return code_;
  }

  [[nodiscard]] std::string string() const;

#define BRIDGE_MAKE_STATUS(code)                                  \
  [[nodiscard]] static constexpr Status make##code(               \
      auto &&...payloads) noexcept {                              \
    return Status{Code::code, std::stacktrace::current(),         \
                  std::forward<decltype(payloads)>(payloads)...}; \
  }

  BRIDGE_MAKE_STATUS(Success);
  BRIDGE_MAKE_STATUS(AccessDenied);
  BRIDGE_MAKE_STATUS(BadAddress);
  BRIDGE_MAKE_STATUS(BadProcess);
  BRIDGE_MAKE_STATUS(BadHandle);
  BRIDGE_MAKE_STATUS(InvalidArgument);
  BRIDGE_MAKE_STATUS(InvalidSignature);
  BRIDGE_MAKE_STATUS(NotFound);
  BRIDGE_MAKE_STATUS(NotImplemented);
  BRIDGE_MAKE_STATUS(SymbolNotFound);
  BRIDGE_MAKE_STATUS(SystemSpecific);
  BRIDGE_MAKE_STATUS(Protection);
  BRIDGE_MAKE_STATUS(Unknown);

#undef BRIDGE_MAKE_STATUS

private:
  friend class StatusFormatter;

  Code code_{};
  std::stacktrace trace_{};
  std::vector<std::string> payloads_{};
};

}  // namespace bridge