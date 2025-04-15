#pragma once
#include <format>  // format

#include <idafucker/CoreDefines.hpp>

#include "Exception.hpp"

IDAFUCKER_NAMESPACE_BEGIN

class PlatformException final : public Exception {
 private:
  [[nodiscard]] static auto platform_error_code()
  {
#if defined(IDAFUCKER_PLATFORM_WIN32)
    return std::format(" [0x{:X}]", ::GetLastError());
#elif defined(IDAFUCKER_PLATFORM_LINUX)
    return "";
#else
  #error "Unsupported platform"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)
  }

 public:
  using Exception::Exception;
  using Exception::operator<<;

  [[nodiscard]] virtual const char* what() const noexcept override
  {
    const_cast<PlatformException*>(this)->message_ += platform_error_code();
    return message_.c_str();
  }
};

IDAFUCKER_NAMESPACE_END