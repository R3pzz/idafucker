#include <idafucker/runtime/ApplicationFactory.hpp>

#if defined(IDAFUCKER_PLATFORM_WIN32)
  #include "impl/application/windows/WindowsApplication.hpp"
using PlatformApplication = idafucker::WindowsApplication;
#else
  #error "Current platform not supported."
#endif // defined(IDAFUCKER_PLATFORM_WIN32)

IDAFUCKER_NAMESPACE_BEGIN

[[nodiscard]] std::shared_ptr<Application> ApplicationFactory::create(int argc, char *argv[])
{
  if (_instance != nullptr)
    throw std::runtime_error{"Application instance already exists"};

  _instance = std::make_shared<PlatformApplication>(argc, argv);
  return _instance;
}

ApplicationFactory applicationFactory{};

IDAFUCKER_NAMESPACE_END