#include "WindowsOpenGLContext.hpp"

IDAFUCKER_NAMESPACE_BEGIN

WindowsOpenGLContext::WindowsOpenGLContext(HWND windowHandle, const OpenGLContextSpecs &specs)
{
  _dc = ::GetDC(windowHandle);
  IDAFUCKER_CHECK_NONZERO(_dc);

  PIXELFORMATDESCRIPTOR pfd{};
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1u;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = static_cast<BYTE>(specs.format);
  pfd.cColorBits = specs.colorDepth;
  pfd.cDepthBits = static_cast<BYTE>(specs.depthBufferSize);
  pfd.cStencilBits = static_cast<BYTE>(specs.stencilBufferSize);
  pfd.iLayerType = PFD_MAIN_PLANE;

  auto pixelFormat = ::ChoosePixelFormat(_dc, &pfd);
  if (!pixelFormat)
    throw std::runtime_error{"System does not support the given pixel format"};
  
  ::SetPixelFormat(_dc, pixelFormat, &pfd);

  _glrc = wglCreateContext(_dc);
  IDAFUCKER_CHECK_NONZERO(_glrc);
}

WindowsOpenGLContext::~WindowsOpenGLContext()
{
  wglDeleteContext(_glrc);
}

bool WindowsOpenGLContext::attach() const
{
  return wglMakeCurrent(_dc, _glrc) == TRUE;
}

bool WindowsOpenGLContext::isAttached() const
{
  return wglGetCurrentContext() == _glrc;
}

void WindowsOpenGLContext::swapBuffers() const
{
  ::SwapBuffers(_dc);
}

IDAFUCKER_NAMESPACE_END