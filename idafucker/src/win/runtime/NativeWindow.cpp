#define FUSE_EXPOSE_SYSTEM_HEADERS
#include "NativeWindow.hpp"
#include "NativeApplication.hpp"
#include "WindowMessageTranslator.hpp"

#include <idafucker/runtime/Window.hpp>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

namespace idafucker::detail
{
NativeWindow::NativeWindow(
    const Application& application, const WindowOptions& options,
    Window* wrapper) {
  // clang-format off
  handle_ = ::CreateWindowExW(
      {}, //< Extended style flags
      MAKEINTATOM(application.nativeApplication->defaultWindowAtom()), //< Window class
      options.title.c_str(), //< Window title
      WS_OVERLAPPEDWINDOW, //< Window style is defaulted to `WS_OVERLAPPEDWINDOW` because there is no need to abstract window style and pass it in `specs`
      0, 0, //< Window position
      options.size.x, options.size.y, //< Window size
      NULL, //< Parent window
      NULL, //< Menu handle
      ::GetModuleHandleW(nullptr), //< Application instance
      reinterpret_cast<LPVOID>(this) //< Current class pointer
  );
  // clang-format on
  if (handle_ == NULL)
    throw std::runtime_error{"failed creating a window"};

  ::SetWindowLongPtrW(
      handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wrapper));

  // Remove the DWM frame - it is useless when we want a custom title bar
  if (options.title.empty()) {
    const MARGINS margins{-1};
    ::DwmExtendFrameIntoClientArea(handle_, &margins);
  }
}

NativeWindow::~NativeWindow() noexcept {
  ::DestroyWindow(handle_);
}

void NativeWindow::show() noexcept {
  if (handle_ != NULL)
    ::ShowWindow(handle_, SW_SHOW);
}

void NativeWindow::hide() noexcept {
  if (handle_ != NULL)
    ::ShowWindow(handle_, SW_HIDE);
}

void NativeWindow::maximize() noexcept {
  if (handle_ != NULL)
    ::ShowWindow(handle_, SW_MAXIMIZE);
}

void NativeWindow::minimize() noexcept {
  if (handle_ != NULL)
    ::ShowWindow(handle_, SW_MINIMIZE);
}

void NativeWindow::restoreDefaultSize() noexcept {
  if (handle_ != NULL)
    ::ShowWindow(handle_, SW_SHOWNORMAL);
}

void NativeWindow::adjustBounds(const Rectangle<int>& rect) noexcept {
  if (handle_ == NULL)
    return;

  RECT target{};
  ::GetWindowRect(handle_, &target);
  if (rect.left != -1)
    target.left = rect.left;
  if (rect.top != -1)
    target.top = rect.top;
  if (rect.right != -1)
    target.right = rect.right;
  if (rect.bottom != -1)
    target.bottom = rect.bottom;

  ::SetWindowPos(
      handle_, NULL, target.left, target.top, target.right - target.left,
      target.bottom - target.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

[[nodiscard]] Rectangle<int> NativeWindow::clientAreaBounds() const noexcept {
  Rectangle<int> bounds{};
  ::GetClientRect(handle_, reinterpret_cast<LPRECT>(&bounds));
  return bounds;
}

[[nodiscard]] int NativeWindow::dpi() const noexcept {
  return handle_ == NULL ? 96 : ::GetDpiForWindow(handle_);
}

LRESULT CALLBACK NativeWindow::nativeMessageHandler(
    HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  auto window =
      reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
  if (window == nullptr)
    return ::DefWindowProcW(hwnd, message, wparam, lparam);

  auto messageWasAcknowledged{false};

  switch (message) {
  case WM_DESTROY:
    messageWasAcknowledged = window->handleCloseEvent();
    break;
  case WM_SIZE:
    messageWasAcknowledged = window->handleSizeChangedEvent(
        messages::SizeMessage::translateSize(lparam),
        messages::SizeMessage::translateType(wparam));
    break;
  default:
    break;
  }

  return messageWasAcknowledged
             ? 0
             : ::DefWindowProcW(hwnd, message, wparam, lparam);
}
}  // namespace idafucker::detail