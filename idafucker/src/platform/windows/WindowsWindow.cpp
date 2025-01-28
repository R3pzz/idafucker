#include <platform/windows/WindowsWindow.hpp>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

IDAFUCKER_NAMESPACE_BEGIN

WindowsAppWindow::WindowsAppWindow(const CreateParams &params, LPCWSTR className){
  const auto instance = ::GetModuleHandleW(nullptr);

  _hwnd = ::CreateWindowExW(
      params.styleEx, className, params.title.c_str(), params.style, params.rect.left, params.rect.top, params.width(),
      params.height(), NULL, NULL, instance, reinterpret_cast<LPVOID>(this));
  IDAFUCKER_CHECK_NONZERO(_hwnd);
  
  if (params.customTitleBar) {
    const MARGINS margins{-1};
    DwmExtendFrameIntoClientArea(_hwnd, &margins);
  }
}

WindowsAppWindow::~WindowsAppWindow() {
  if (_hwnd != NULL)
    ::DestroyWindow(_hwnd);
  _hwnd = nullptr;
}

void WindowsAppWindow::setVisibility(bool visible) { ::ShowWindow(_hwnd, visible ? SW_SHOW : SW_HIDE); }

LRESULT WINAPI WindowsAppWindow::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

IDAFUCKER_NAMESPACE_END