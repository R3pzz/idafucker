#include "WindowsWindow.hpp"

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

IDAFUCKER_NAMESPACE_BEGIN

WindowsWindow::WindowsWindow(const WindowSpecs &specs, ATOM windowClassAtom)
{
  const auto instance = ::GetModuleHandleW(nullptr);
  IDAFUCKER_CHECK_NONZERO(instance);

  _handle = ::CreateWindowExW({},
      MAKEINTATOM(windowClassAtom),
      specs.title.c_str(),
      WS_OVERLAPPEDWINDOW,
      0, 0,
      static_cast<int>(specs.extent.x),
      static_cast<int>(specs.extent.y),
      NULL, NULL, instance, reinterpret_cast<LPVOID>(this));
  IDAFUCKER_CHECK_NONZERO(_handle);

  if (!specs.hasTitleBar()) {
    const MARGINS margins{-1};
    DwmExtendFrameIntoClientArea(_handle, &margins);
  }
}

WindowsWindow::~WindowsWindow()
{
  ::DestroyWindow(_handle);
  _handle = nullptr;
}

void WindowsWindow::changeVisibility(WindowVisibility value)
{
  if (_visibility == value)
    return;

  switch (value) {
  case WindowVisibility::Shown: ::ShowWindow(_handle, SW_SHOWDEFAULT); break;
  case WindowVisibility::Hidden: ::ShowWindow(_handle, SW_HIDE); break;
  case WindowVisibility::Minimized: ::ShowWindow(_handle, SW_MINIMIZE); break;
  case WindowVisibility::Maximized: ::ShowWindow(_handle, SW_MAXIMIZE); break;
  default:
    spdlog::debug("WindowVisibility::'{}' not supported for window at '{:X}'",
        static_cast<int>(value),
        reinterpret_cast<std::uintptr_t>(this));
    break;
  }

  _visibility = value;
}

[[nodiscard]] std::wstring WindowsWindow::title() const
{
  std::wstring result{};
  result.resize(static_cast<std::size_t>(::GetWindowTextLengthW(_handle)));
  ::GetWindowTextW(_handle, result.data(), static_cast<int>(result.size()));
  return result;
}

void WindowsWindow::changeTitle(const std::wstring &title)
{
  ::SetWindowTextW(_handle, title.c_str());
}

[[nodiscard]] std::weak_ptr<OpenGLContext> WindowsWindow::createOpenGLContext(const OpenGLContextSpecs &specs)
{
  _glContext = std::make_shared<WindowsOpenGLContext>(_handle, specs);
  return _glContext;
}

IDAFUCKER_NAMESPACE_END