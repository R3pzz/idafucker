#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <fuse/CommandLine.hpp>
#undef FUSE_EXPOSE_SYSTEM_HEADERS

namespace fuse
{
[[nodiscard]] CommandLine<wchar_t> systemCommandLine() noexcept {
#if FUSE_PLATFORM_WIN
  return CommandLine<wchar_t>{::GetCommandLineW()};
#elif FUSE_PLATFORM_LINUX
  #error "Not implemented yet..."
#else
  #error "Unsupported platform"
#endif
}
}  // namespace fuse