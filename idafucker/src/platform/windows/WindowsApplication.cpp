#include <idafucker/platform/windows/WindowsApplication.hpp>

IDAFUCKER_NAMESPACE_BEGIN

WindowsApp::WindowsApp(int argc, char *argv[]) {
  // Parse command line here

  // Register window class
  WNDCLASSW wc{
      .lpfnWndProc{&WindowsAppWindow::wndProc},
      .hInstance{::GetModuleHandleW(nullptr)},
      .lpszClassName{windowClassName()}};
  IDAFUCKER_CHECK_NONZERO(::RegisterClassW(&wc));
}

WindowsApp::~WindowsApp() {
  const auto instance{::GetModuleHandleW(nullptr)};
  ::UnregisterClassW(windowClassName(), instance);
}

[[nodiscard]] WindowsAppWindow &WindowsApp::createWindow(const WindowsAppWindow::CreateParams &params) {
  auto &window = _windows.emplace_back(params, windowClassName());
  ::SetWindowLongPtrW(window.hwnd(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  return window;
}

IDAFUCKER_NAMESPACE_END