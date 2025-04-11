#include "ApplicationImpl.hpp"

#include <idafucker/exceptions/PlatformException.hpp>

#include "./detail/Character.hpp"
#include "./detail/WindowClass.hpp"
#include "MessageProcessorImpl.hpp"
#include "WindowImpl.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace win {
ApplicationImpl::ApplicationImpl(int argc, char* argv[])
{
  // Parse command line

  // Register window class
  registerWindowClass();
}

ApplicationImpl::~ApplicationImpl()
{
  unregisterWindowClass();
}

[[nodiscard]] Window::Ref ApplicationImpl::makeWindow(const WindowSpecs& specs)
{
  auto& window = windows_.emplace_back(
      std::make_shared<WindowImpl>(specs, mainClassAtom_));
  onNewWindow.emit(window);
  return window;
}

void ApplicationImpl::registerWindowClass()
{
  constexpr auto name = WINDOWS_STRING("IDAFucker.Window");
  constexpr auto style = detail::WindowClassStyle::RedrawOnSizeChanged;

  ::WNDCLASSEX wc{
      .cbSize = sizeof(::WNDCLASSEX),
      .style = style,
      .lpfnWndProc = &MessageProcessorImpl::windowProcess,
      .hInstance = ::GetModuleHandle(nullptr),
      .hIcon = NULL,
      .hCursor = NULL,
      .lpszClassName = name,
  };

  mainClassAtom_ = ::RegisterClassEx(&wc);
  if (mainClassAtom_ == NULL) {
    throw PlatformException{"Failed to register window class"};
  }
}

void ApplicationImpl::unregisterWindowClass() noexcept
{
  ::UnregisterClass(MAKEINTATOM(mainClassAtom_), ::GetModuleHandle(nullptr));
}
}  // namespace win

IDAFUCKER_NAMESPACE_END