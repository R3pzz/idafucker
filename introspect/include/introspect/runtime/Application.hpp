#pragma once
#include <introspect/Config.hpp>

#include <fuse/Class.hpp>
#include <fuse/CommandLine.hpp>
#include <fuse/Signal.hpp>

namespace introspect
{

namespace detail
{

class NativeApplication;
class NativeWindow;

}  // namespace detail

class Application final {
public:
  Application(const fuse::CommandLine<wchar_t> &commandLine);
  ~Application();

  constexpr void terminate() noexcept {
    terminating_ = true;
  }

  void runEventLoop(const std::function<void()> &loopFunc) noexcept;

  fuse::Signal<void()>
      terminationEvent{};  //< This event gets emitted when `runEventLoop()`
                           // starts the termination sequence or when
                           //`~ApplicationImpl()` gets called.

private:
  bool terminating_{false};
  std::unique_ptr<detail::NativeApplication> native_;

  friend class detail::NativeWindow;

  FUSE_NONCOPYABLE(Application);
};

}  // namespace introspect