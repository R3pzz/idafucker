#include <idafucker/runtime/WindowFactory.hpp>

#if defined(IDAFUCKER_PLATFORM_WIN32)
  #include "impl/window/windows/WindowsWindow.hpp"
  #include "impl/window/windows/WindowsWindowFactoryImpl.hpp"
  using PlatformWindowFactoryImpl = idafucker::WindowsWindowFactoryImpl;
#else
  #error "Current platform not supported."
#endif // defined(IDAFUCKER_PLATFORM_WIN32)

IDAFUCKER_NAMESPACE_BEGIN

WindowFactory::WindowFactory() : _impl{std::make_unique<PlatformWindowFactoryImpl>()}
{
}

[[nodiscard]] std::shared_ptr<Window> WindowFactory::create(const WindowSpecs& specs) const
{
#if defined(IDAFUCKER_PLATFORM_WIN32)
  auto impl = reinterpret_cast<WindowsWindowFactoryImpl *>(_impl.get());
  return std::make_shared<WindowsWindow>(specs, impl->windowClassAtom());
#else
  #error "Current platform not supported."
#endif // defined(IDAFUCKER_PLATFORM_WIN32)
}

WindowFactory windowFactory{};

IDAFUCKER_NAMESPACE_END