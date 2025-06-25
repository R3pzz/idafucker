#pragma once
#include <format>  // format

#include <bridge/shared/Status.hpp>
#include <fuse/Enum.hpp>

namespace bridge
{

class StatusFormatter final {
public:
  [[nodiscard]] static std::string format(const Status &status);

private:
  [[nodiscard]] static constexpr std::string stringify(
      Status::Code code) noexcept {
    switch (code) {
    case Status::Code::Success:
      return "Success";
    case Status::Code::AccessDenied:
      return "Access Denied";
    case Status::Code::BadAddress:
      return "Bad Address";
    case Status::Code::BadProcess:
      return "Bad Process";
    case Status::Code::BadHandle:
      return "Bad Handle";
    case Status::Code::InvalidArgument:
      return "Invalid Argument";
    case Status::Code::InvalidSignature:
      return "Invalid Signature";
    case Status::Code::NotFound:
      return "NotFound";
    case Status::Code::NotImplemented:
      return "NotImplemented";
    case Status::Code::SymbolNotFound:
      return "SymbolNotFound";
    case Status::Code::SystemSpecific:
      return "SystemSpecific";
    case Status::Code::Protection:
      return "Protection";
    case Status::Code::Unknown:
      return "Unknown";
    default:
      return "<unspecified>";
    }
  }

  [[nodiscard]] static std::string format(const std::stacktrace &trace);
  [[nodiscard]] static std::string format(const std::string &string);
};

}  // namespace bridge