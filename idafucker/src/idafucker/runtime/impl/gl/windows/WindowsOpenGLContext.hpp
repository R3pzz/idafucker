#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/OpenGLContext.hpp>
#include <idafucker/runtime/OpenGLContextSpecs.hpp>

IDAFUCKER_NAMESPACE_BEGIN

class WindowsOpenGLContext final : public OpenGLContext {
public:
  WindowsOpenGLContext(HWND windowHandle, const OpenGLContextSpecs &specs);
  virtual ~WindowsOpenGLContext() override;

  virtual bool attach() const override;
  [[nodiscard]] virtual bool isAttached() const override;

  virtual void swapBuffers() const override;

private:
  HDC _dc{};
  HGLRC _glrc{};
};

IDAFUCKER_NAMESPACE_END