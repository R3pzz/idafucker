#pragma once
#include <idafucker/CoreDefines.hpp>

//
// Abstract OpenGL context class. This is basically an interface
// to platform's windowing system, that is implemented differently
// for different platforms(WGL for Windows, EGL for Linux[wayland]).
//

IDAFUCKER_NAMESPACE_BEGIN

class OpenGLContext {
public:
  virtual ~OpenGLContext() = default;
  
  // Make the context current
  virtual bool attach() const = 0;
  // Is this context the current one?
  [[nodiscard]] virtual bool isAttached() const = 0;

  // Swap the back and the front buffers
  virtual void swapBuffers() const = 0;
};

IDAFUCKER_NAMESPACE_END