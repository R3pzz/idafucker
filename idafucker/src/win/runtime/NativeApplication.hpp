#pragma once
#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <idafucker/Config.hpp>
#include <idafucker/runtime/Application.hpp>

#include <fuse/CommandLine.hpp>

namespace idafucker::detail
{
class NativeApplication final {
public:
  static constexpr auto kClassName{L"IDAFUCKER"};

  explicit NativeApplication(const fuse::CommandLine<wchar_t> &commandLine);
  ~NativeApplication() noexcept;

  [[nodiscard]] constexpr ATOM defaultWindowAtom() const noexcept {
    return winAtom_;
  }

  void runEventLoop(const std::function<void()> &loopFunc) noexcept;
  
private:
  void registerClass();
  void unregisterClass() noexcept;
  void configureDpi();

  ATOM winAtom_;

  FUSE_NONCOPYABLE(NativeApplication);
};
}  // namespace idafucker::detail