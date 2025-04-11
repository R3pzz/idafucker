#pragma once
#include <functional>  // function
#include <memory>      // shared_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/CommandLine.hpp>
#include <idafucker/base/Signal.hpp>

#include "Types.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace impl::win {
// Windows application implementation.
class ApplicationImpl final {
  IDAFUCKER_NONCOPYABLE(ApplicationImpl);

 public:
  ApplicationImpl();
  ~ApplicationImpl();

  [[nodiscard]] constexpr ATOM wcAtom() const noexcept
  {
    return wcAtom_;
  }

  constexpr void terminate() noexcept
  {
    awaitingTermination_ = true;
  }

  void runEventLoop() noexcept;

  // Events(these are freely accessible in other parts of code):
  Signal<void(CommandLine<Character, Delimiter>)> initializationEvent{};
  Signal<void()>
      terminationEvent{};  //< This event gets emitted when `runEventLoop()`
                           //starts the termination sequence or when
                           //`~ApplicationImpl()` gets called.

 private:
  // Window class helpers
  void registerClass(View name);
  void unregisterClass() noexcept;

  ATOM wcAtom_;
  bool awaitingTermination_{false};

  // Creates the DPI context for this process
  static void configureDpi();

  // A window message handler
  [[nodiscard]] static LRESULT CALLBACK windowProc(HWND hwnd, UINT message,
                                                   WPARAM wparam,
                                                   LPARAM lparam);
};
}  // namespace impl::win

IDAFUCKER_NAMESPACE_END