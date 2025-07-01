#pragma once
#include <expected>    // expected
#include <format>      // formatter
#include <stacktrace>  // stacktrace

#include <fuse/Enum.hpp>
#include <fuse/Platform.hpp>
#include <fuse/TypeTraits.hpp>

namespace bridge::win
{

struct Status {
public:
  enum class Code {
    Success,         //< States a successful outcome of an operation.
    BadArgument,     //< A function received an invalid/unexpected argument.
    BadHandle,       //< Function encontered a potentially bad/uninitialized handle.
    NotFound,        //< The desired object could not be found.
    NotImplemented,  //< Some part of logic/code/etc... is not implemented.
    OS,              //< Inidicates that the error comes from the OS. Progagates the OS
                     // error code.
  };

  static constexpr DWORD k_badSystemErrorCode{~0u};

  // Construct a `Status` object with a `Success` code.
  constexpr Status() noexcept = default;

  // Construct a `Status` object with a given code and an associated stack
  // trace.
  Status(Code code, std::stacktrace trace = std::stacktrace::current()) noexcept
      : code_{std::move(code)}, trace_{std::move(trace)} {}

  // Construct a `Status` object with a given code, an associated stack
  // trace and a system error code.
  Status(Code code,
         DWORD systemErrorCode,
         std::stacktrace trace = std::stacktrace::current()) noexcept
      : code_{std::move(code)},
        trace_{std::move(trace)},
        systemErrorCode_{std::move(systemErrorCode)} {}

  [[nodiscard]] constexpr bool operator!() const noexcept {
    return code_ != Code::Success;
  }

  template <typename Type>
  [[nodiscard]] constexpr operator std::expected<Type, Status>() const& noexcept {
    return std::unexpected{*this};
  }

  template <typename Type>
  [[nodiscard]] constexpr operator std::expected<Type, Status>() const&& noexcept {
    return std::unexpected{std::move(*this)};
  }

  [[nodiscard]] constexpr bool success() const noexcept {
    return code_ == Code::Success;
  }

  [[nodiscard]] constexpr Code code() const noexcept {
    return code_;
  }

  [[nodiscard]] std::string explain() const noexcept {
    switch (code_) {
    case Code::Success:
      return "Operation was successful";
    case Code::BadArgument:
      return "Either one or more of the provided arguments was/were unexpected";
    case Code::BadHandle:
      return "The provided handle was invalid";
    case Code::NotFound:
      return "The object in search could not be found";
    case Code::NotImplemented:
      return "The logic is not implemented";
    case Code::OS:
      assert(systemErrorCode_);
      return std::format("Execution encountered an OS error (code `0x{:X}`)",
                         systemErrorCode_);
    default:
      assert(false, "the error code supplied in `code_` was not recognized");
      std::unreachable();
    }
  }

#define BRIDGE_WIN_MAKE_STATUS(status)                  \
  [[nodiscard]] static Status make##status() noexcept { \
    return Status{Code::status};                        \
  }

  BRIDGE_WIN_MAKE_STATUS(Success);
  BRIDGE_WIN_MAKE_STATUS(BadArgument);
  BRIDGE_WIN_MAKE_STATUS(BadHandle);
  BRIDGE_WIN_MAKE_STATUS(NotFound);
  BRIDGE_WIN_MAKE_STATUS(NotImplemented);

#undef BRIDGE_WIN_MAKE_STATUS

  [[nodiscard]] static Status makeOS() noexcept {
    return Status{Code::OS, ::GetLastError(), std::stacktrace::current()};
  }

  [[nodiscard]] static Status makeOS(NTSTATUS status) noexcept {
    return Status{Code::OS, std::move<DWORD>(status), std::stacktrace::current()};
  }

public:
  Code code_{};
  std::stacktrace trace_{};
  DWORD systemErrorCode_{k_badSystemErrorCode};
};

#define BRIDGE_WIN_CHECK_NULL(expr, status) \
  if ((expr) == NULL) [[unlikely]] {        \
    return status;                          \
  }

#define BRIDGE_WIN_CHECK_OS(expr)    \
  if (FAILED((expr))) [[unlikely]] { \
    return Status::makeOS();         \
  }

#define BRIDGE_WIN_CHECK_NT(expr)                                      \
  if (const auto ntStatus = (expr); NT_ERROR(ntStatus)) [[unlikely]] { \
    return Status::makeOS(ntStatus);                                   \
  }

template <typename Type>
using StatusOr = std::expected<Type, Status>;

}  // namespace bridge::win

namespace std
{

template <>
struct formatter<bridge::win::Status> {
  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const bridge::win::Status& status, FormatContext& ctx) const {
    using Code = bridge::win::Status::Code;

    std::string_view codeStr{"<unknown>"};
    switch (status.code()) {
    case Code::Success:
      codeStr = "Success";
      break;
    case Code::BadArgument:
      codeStr = "BadArgument";
      break;
    case Code::BadHandle:
      codeStr = "BadHandle";
      break;
    case Code::NotImplemented:
      codeStr = "NotImplemented";
      break;
    case Code::OS:
      codeStr = "OS";
      break;
    }

    if (status.code() == Code::OS) {
      return std::format_to(ctx.out(),
                            "Status: `{}` (code:{} | system:0x{:X});\nStacktrace:\n{}",
                            codeStr, fuse::underlyingCast(status.code()),
                            status.systemErrorCode_, std::to_string(status.trace_));
    } else {
      return std::format_to(ctx.out(), "Status: `{}` (code:{});\nStacktrace:\n{}",
                            codeStr, fuse::underlyingCast(status.code()),
                            std::to_string(status.trace_));
    }
  }
};

}  // namespace std