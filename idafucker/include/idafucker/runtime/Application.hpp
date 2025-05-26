#pragma once
#include <fuse/Class.hpp>
#include <fuse/CommandLine.hpp>
#include <fuse/Signal.hpp>
#include <idafucker/Config.hpp>

namespace idafucker
{
namespace detail
{
class NativeApplication;
class NativeWindow;
} // namespace detail

class Application final {
public:
  Application(const fuse::CommandLine<wchar_t> &commandLine);
  ~Application();

  constexpr void terminate() noexcept {
    awaitingTermination = true;
  }

  void runEventLoop(const std::function<void()> &loopFunc) noexcept;

  fuse::Signal<void()>
      terminationEvent{};  //< This event gets emitted when `runEventLoop()`
                           // starts the termination sequence or when
                           //`~ApplicationImpl()` gets called.

private:
  bool awaitingTermination{false};
  std::unique_ptr<detail::NativeApplication> nativeApplication;

  friend class detail::NativeWindow;

  FUSE_NONCOPYABLE(Application);
};
} // namespace idafucker