#include <introspect/runtime/Application.hpp>

#if FUSE_PLATFORM_WIN
  #include "../../win/runtime/NativeApplication.hpp"
#else
  #error "Not supported yet..."
#endif

namespace introspect
{

Application::Application(const fuse::CommandLine<wchar_t> &commandLine)
    : native_{new detail::NativeApplication{commandLine}} {}

Application::~Application() {
  terminationEvent.emit();
}

void Application::runEventLoop(const std::function<void()> &loopFunc) noexcept {
  while (!terminating_)
    native_->runEventLoop(loopFunc);
}

}  // namespace introspect