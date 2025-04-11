#include "WindowImpl.hpp"

#include <dwmapi.h>

#include <idafucker/base/String.hpp>
#include <idafucker/exceptions/PlatformException.hpp>

#include "detail/DPI.hpp"
#pragma comment(lib, "Dwmapi.lib")

IDAFUCKER_NAMESPACE_BEGIN

namespace win {
WindowImpl::WindowImpl(const WindowSpecs& specs, ATOM atom)
    : messageProcessor_{}
{
  const auto instance = ::GetModuleHandleW(nullptr);
  const auto title = widen(specs.title);

  // clang-format off
  handle_ = ::CreateWindowExW(
      {}, //< Extended style flags
      MAKEINTATOM(atom), //< Window class
      title.c_str(), //< Window title
      WS_OVERLAPPEDWINDOW, //< Window style is defaulted to `WS_OVERLAPPEDWINDOW` because there is no need to abstract window style and pass it in `specs`
      0, 0, //< Window position
      specs.size.x, specs.size.y, //< Window size
      NULL, //< Parent window
      NULL, //< Menu handle
      instance, //< Application instance
      reinterpret_cast<LPVOID>(this) //< Current class pointer
  );
  // clang-format on
  if (handle_ == NULL) {
    throw PlatformException{"Failed to create a window `"} << specs.title
                                                           << "`";
  }

  ::SetWindowLongPtr(handle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

  // Custom title bar
  if (!specs.hasTitleBar()) {
    const MARGINS margins{-1};
    DwmExtendFrameIntoClientArea(handle_, &margins);
  }

  // DPI support
  if (specs.enableDpiSupport) {
    configureDpi();
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

  ::SetWindowPos(handle_, NULL, target.left, target.top,
                 target.right - target.left, target.bottom - target.top,
                 SWP_NOZORDER | SWP_NOACTIVATE);
}

[[nodiscard]] bool WindowImpl::isDpiSupported() const noexcept
{
  return ::IsProcessDPIAware() == TRUE;
}

[[nodiscard]] int WindowImpl::dpi() const noexcept
{
  if (handle_ != NULL) {
    return ::GetDpiForWindow(handle_);
  }
  return 96;
}

[[nodiscard]] MessageProcessor& WindowImpl::messageProcessor() noexcept
{
  return messageProcessor_;
}

[[nodiscard]] void* WindowImpl::platformHandle() const noexcept
{
  return reinterpret_cast<void*>(handle_);
}

void WindowImpl::configureDpi()
{
  if (::SetProcessDpiAwarenessContext(
          DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE) != TRUE) {
    throw PlatformException{"Failed to set process DPI awareness context"};
  }

  if (const auto result = detail::SetProcessDpiAwareness(
          detail::ProcessDpiAwareness::PerMonitorDPIAware);
      result != S_OK && result != E_ACCESSDENIED) {
    throw PlatformException{"Failed to set process DPI awareness"};
  }

  if (::SetProcessDPIAware() != TRUE) {
    throw PlatformException{"Failed to set process DPI aware"};
  }
}
}  // namespace win

IDAFUCKER_NAMESPACE_END