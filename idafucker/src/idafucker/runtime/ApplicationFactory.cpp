#include <idafucker/exceptions/Exception.hpp>
#include <idafucker/runtime/ApplicationFactory.hpp>

#if defined(IDAFUCKER_PLATFORM_WIN32)
  #include "win/ApplicationImpl.hpp"
using Impl = idafucker::win::ApplicationImpl;
#else
  #error "Current platform not supported."
#endif  // defined(IDAFUCKER_PLATFORM_WIN32)

IDAFUCKER_NAMESPACE_BEGIN

[[nodiscard]] Application::Ref ApplicationFactory::create(int argc, char* argv[])
{
  if (instance_ != nullptr) {
    throw Exception{"Application instance already exists"};
  }

  instance_ = std::make_shared<Impl>(argc, argv);
  return instance_;
}

IDAFUCKER_NAMESPACE_END