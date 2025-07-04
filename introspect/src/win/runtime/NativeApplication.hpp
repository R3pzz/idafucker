#pragma once
#include <introspect/Config.hpp>
#include <introspect/runtime/Application.hpp>

#include <fuse/CommandLine.hpp>
#include <fuse/Platform.hpp>

namespace introspect::detail
{

class NativeApplication final {
public:
  static constexpr auto k_className{L"introspect::main"};

  explicit NativeApplication(const fuse::CommandLine<wchar_t> &commandLine);
  ~NativeApplication() noexcept;

  [[nodiscard]] constexpr ATOM atom() const noexcept {
    return atom_;
  }

  void runEventLoop(const std::function<void()> &loopFunc) noexcept;

private:
  void registerClass();
  void unregisterClass() noexcept;
  void configureDpi();

  ATOM atom_;

  FUSE_NONCOPYABLE(NativeApplication);
};

}  // namespace introspect::detail