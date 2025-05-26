#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <idafucker/runtime/Application.hpp>

#if FUSE_PLATFORM_WIN
  #include "../../win/runtime/NativeApplication.hpp"
#else
  #error "Not supported yet..."
#endif

namespace idafucker
{

Application::Application(const fuse::CommandLine<wchar_t> &commandLine)
    : nativeApplication{new detail::NativeApplication{commandLine}} {}

Application::~Application() {
  terminationEvent.emit();
}

void Application::runEventLoop(const std::function<void()> &loopFunc) noexcept {
  while (!awaitingTermination)
    nativeApplication->runEventLoop(loopFunc);
}

}  // namespace idafucker