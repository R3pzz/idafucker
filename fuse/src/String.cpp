#include <fuse/String.hpp>

#include <Windows.h> // MultiByteToWideChar, WideCharToMultiByte

namespace fuse
{
[[nodiscard]] std::wstring ToUnicode::impl(
    const char* data, const std::size_t size) const {
#if FUSE_PLATFORM_WIN
  std::wstring converted{};

  const auto convertedSize = ::MultiByteToWideChar(
      CP_UTF8, NULL, data, static_cast<int>(size), nullptr, 0);
  if (convertedSize < 0)
    throw std::runtime_error{"bad wide char cast"};

  converted.resize(static_cast<std::size_t>(convertedSize));
  if (::MultiByteToWideChar(
          CP_UTF8, NULL, data, size, converted.data(), converted.size()) < 0)
    throw std::runtime_error{"bad wide char cast"};

  return converted;
#elif FUSE_PLATFORM_LINUX
  #error "Not implemented yet..."
#else
  #error "Platform not supported"
#endif
}

[[nodiscard]] std::string ToUtf8::impl(
    const wchar_t* data, const std::size_t size) const {
#if FUSE_PLATFORM_WIN
  std::string converted{};

  const auto convertedSize = ::WideCharToMultiByte(
      CP_UTF8, NULL, data, static_cast<int>(size), nullptr, 0, NULL, NULL);
  if (convertedSize < 0)
    throw std::runtime_error{"bad multibyte cast"};

  converted.resize(static_cast<std::size_t>(convertedSize));
  if (::WideCharToMultiByte(
          CP_UTF8, NULL, data, size, converted.data(), converted.size(), NULL,
          NULL) < 0)
    throw std::runtime_error{"bad multibyte cast"};

  return converted;
#elif FUSE_PLATFORM_LINUX
  #error "Not implemented yet..."
#else
  #error "Platform not supported"
#endif
}
}  // namespace fuse