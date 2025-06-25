#include <fuse/CommandLine.hpp>
#include <fuse/Platform.hpp>

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