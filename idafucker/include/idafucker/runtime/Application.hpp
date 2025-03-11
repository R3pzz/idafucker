#pragma once
#include <idafucker/CoreDefines.hpp>
#include "WindowSpecs.hpp"
#include "Window.hpp"

#include <memory> // shared_ptr

//
// Application is a simple and easy-to-use interface of all underlying OS calls, such as:
// 
//  - Taskbar operations
//  - System object registration
//  - Config parsing
//  - Window management
//  - Rendering management
//
// and etc... We can call it a 'global application context'.
//
// An application is created by a factory that can only create one instance of the application
// at the runtime. A factory implements a method for creating an underlying platform implementation
// of the application.
//

IDAFUCKER_NAMESPACE_BEGIN

class Application {
public:
  virtual ~Application() = default;

  // Should we run the main loop the next tick?
  [[nodiscard]] virtual bool running() const = 0;
  
  // Enqueue an application termination request
  virtual void requestTermination() = 0;
  [[nodiscard]] virtual bool terminationRequested() const = 0;
  
  //
  // This should be replaced with a proper WindowManager class
  //

  // Window ownership
  virtual void registerWindow(const std::shared_ptr<Window> &window) = 0;
  virtual void releaseWindow(const std::shared_ptr<Window> &window) = 0;
  
  // Specific window classes
  [[nodiscard]] virtual std::shared_ptr<Window> primaryWindow() const = 0;
  [[nodiscard]] virtual bool isPrimaryWindow(const std::shared_ptr<Window> &window) const = 0;
};

IDAFUCKER_NAMESPACE_END