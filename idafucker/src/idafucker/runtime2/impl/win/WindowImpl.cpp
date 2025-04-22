#include <dwmapi.h>

#include <idafucker/base/String.hpp>
#include <idafucker/exceptions/PlatformException.hpp>
#include <idafucker/runtime2/impl/win/WindowImpl.hpp>
#pragma comment(lib, "Dwmapi.lib")

IDAFUCKER_NAMESPACE_BEGIN

namespace impl::win
{
WindowImpl::WindowImpl(const WindowOptionsImpl& options)
{
  // clang-format off
  handle_ = ::CreateWindowExW(
      {}, //< Extended style flags
      MAKEINTATOM(options.atom), //< Window class
      options.title.c_str(), //< Window title
      WS_OVERLAPPEDWINDOW, //< Window style is defaulted to `WS_OVERLAPPEDWINDOW` because there is no need to abstract window style and pass it in `specs`
      0, 0, //< Window position
      options.size.x, options.size.y, //< Window size
      NULL, //< Parent window
      NULL, //< Menu handle
      ::GetModuleHandle(nullptr), //< Application instance
      reinterpret_cast<LPVOID>(this) //< Current class pointer
  );
  // clang-format on
  if (handle_ == NULL) {
    throw PlatformException{"Failed to create a window `"}
        << convertToUtf8(options.title) << "`";
  }

  ::SetWindowLongPtr(handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

  // Custom title bar
  if (options.title.empty()) {
    const MARGINS margins{-1};
    DwmExtendFrameIntoClientArea(handle_, &margins);
  }
}

WindowImpl::~WindowImpl()
{
  if (handle_ != NULL) {
    ::DestroyWindow(handle_);
    handle_ = nullptr;
  }
}

void WindowImpl::show() noexcept
{
  if (handle_ != NULL) {
    ::ShowWindow(handle_, SW_SHOW);
  }
}

void WindowImpl::hide() noexcept
{
  if (handle_ != NULL) {
    ::ShowWindow(handle_, SW_HIDE);
  }
}

void WindowImpl::maximize() noexcept
{
  if (handle_ != NULL) {
    ::ShowWindow(handle_, SW_MAXIMIZE);
  }
}

void WindowImpl::minimize() noexcept
{
  if (handle_ != NULL) {
    ::ShowWindow(handle_, SW_MINIMIZE);
  }
}

void WindowImpl::restoreDefaultSize() noexcept
{
  if (handle_ != NULL) {
    ::ShowWindow(handle_, SW_SHOWNORMAL);
  }
}

void WindowImpl::adjustBounds(const Rectangle<int>& rect) noexcept
{
  if (handle_ == NULL) {
    return;
  }

  RECT target{};
  ::GetWindowRect(handle_, &target);

  if (rect.left != -1) {
    target.left = rect.left;
  }
  if (rect.top != -1) {
    target.top = rect.top;
  }
  if (rect.right != -1) {
    target.right = rect.right;
  }
  if (rect.bottom != -1) {
    target.bottom = rect.bottom;
  }

  ::SetWindowPos(
      handle_, NULL, target.left, target.top, target.right - target.left,
      target.bottom - target.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

[[nodiscard]] int WindowImpl::dpi() const noexcept
{
  if (handle_ != NULL) {
    return ::GetDpiForWindow(handle_);
  }
  return 96;
}

[[nodiscard]] HWND WindowImpl::platformHandle() const noexcept
{
  return handle_;
}

LRESULT WindowImpl::onWindowMesasge(UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message) {
    case WM_SIZE:
      boundsChangedEvent.emit(std::make_shared<SizeChangedEvent>(
          static_cast<SizeChangedEvent::Type>(wparam),
          Size{LOWORD(lparam), HIWORD(lparam)}));
      break;
    case WM_KEYDOWN:
    case WM_KEYUP:
      keyboardEvent.emit(std::make_shared<KeyboardEvent>(
          wparam, message == WM_KEYDOWN ? KeyboardEvent::State::Pressed
                                        : KeyboardEvent::State::Released));
      break;
    case WM_DESTROY:
      closeEvent.emit();
      break;
    default:
      break;
  }

  return ::DefWindowProc(handle_, message, wparam, lparam);
}
}  // namespace impl::win

IDAFUCKER_NAMESPACE_END