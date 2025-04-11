#pragma once
#include <functional>  // function
#include <memory>      // shared_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/Signal.hpp>

#include "Window.hpp"
#include "WindowSpecs.hpp"

//
// Application is a simple and easy-to-use interface of all underlying OS calls,
// such as:
//
//  - Taskbar operations
//  - System object registration
//  - Config parsing
//  - Window management
//  - Rendering management
//
// and etc... We can call it a 'global application context'.
//
// An application is created by a factory that can only create one instance of
// the application at the runtime. A factory implements a method for creating an
// underlying platform implementation of the application.
//

IDAFUCKER_NAMESPACE_BEGIN

class Application {
 public:
  using Ref = std::shared_ptr<Application>;

  constexpr Application() noexcept = default;
  virtual ~Application() noexcept = default;

  [[nodiscard]] constexpr bool running() const noexcept
  {
    return running_;
  }

  constexpr void terminate() noexcept
  {
    running_ = false;
  }

  // Windowing interface
  [[nodiscard]] virtual Window::Ref makeWindow(const WindowSpecs& specs) = 0;

  // Callbacks
  Signal<void()> onTerminate{};
  
 private:
  bool running_{true};
};

IDAFUCKER_NAMESPACE_END