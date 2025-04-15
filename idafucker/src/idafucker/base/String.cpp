#include <idafucker/base/String.hpp>
#include <idafucker/exceptions/PlatformException.hpp>

IDAFUCKER_NAMESPACE_BEGIN

[[nodiscard]] std::wstring ToWide::impl(
    const char* data, const std::size_t size) const
{
#if defined(IDAFUCKER_PLATFORM_WIN32)
  std::wstring converted{};

  const auto convertedSize = ::MultiByteToWideChar(
      CP_UTF8, NULL, data, static_cast<int>(size), nullptr, 0);
  if (convertedSize < 0) {
    throw PlatformException{"Failed to convert `"} << data << "` to Unicode";
  }

  converted.resize(static_cast<std::size_t>(convertedSize));
  if (::MultiByteToWideChar(
          CP_UTF8, NULL, data, size, converted.data(), converted.size()) < 0) {
    throw PlatformException{"Failed to convert `"} << data << "` to Unicode";
  }

  return converted;
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)
}

[[nodiscard]] std::string ToNarrow::impl(
    const wchar_t* data, const std::size_t size) const
{
#if defined(IDAFUCKER_PLATFORM_WIN32)
  std::string converted{};

  const auto convertedSize = ::WideCharToMultiByte(
      CP_UTF8, NULL, data, static_cast<int>(size), nullptr, 0, NULL, NULL);
  if (convertedSize < 0) {
    throw PlatformException{"Failed to convert data at `"}
        << decor::hex << reinterpret_cast<std::uintptr_t>(data) << "` to UTF-8";
  }

  converted.resize(static_cast<std::size_t>(convertedSize));
  if (::WideCharToMultiByte(
          CP_UTF8, NULL, data, size, converted.data(), converted.size(), NULL,
          NULL) < 0) {
    throw PlatformException{"Failed to convert data at `"}
        << decor::hex << reinterpret_cast<std::uintptr_t>(data) << "` to UTF-8";
  }

  return converted;
#elif defined(IDAFUCKER_PLATFORM_LINUX)
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)
}

IDAFUCKER_NAMESPACE_END