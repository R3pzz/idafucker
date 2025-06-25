#pragma once
#include <filesystem> // path

#include <fuse/Memory.hpp>
#include <fuse/Platform.hpp>
#include <fuse/String.hpp>

namespace bridge::win
{

struct UnicodeString {
  constexpr UnicodeString() noexcept = default;

  [[nodiscard]] constexpr std::wstring wstring() const noexcept {
    constexpr auto k_bytesInOneChar{sizeof(WCHAR)};
    return std::wstring{buffer, length / k_bytesInOneChar};
  }

  [[nodiscard]] constexpr std::string string() const noexcept {
    return fuse::utf8Cast(buffer);
  }

  USHORT length{};
  USHORT maximumLength{};
  PWSTR buffer{};
};

} // namespace bridge::win