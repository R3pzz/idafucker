#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <idafucker/runtime/Window.hpp>

#if FUSE_PLATFORM_WIN
  #include "../../win/runtime/NativeWindow.hpp"
#elif FUSE_PLATFORM_LINUX
  #error "Not supported yet..."
#endif

namespace idafucker
{

Window::Window(const Application& application, const WindowOptions& options)
    : nativeWindow{new detail::NativeWindow{application, options, this}} {}

Window::~Window() {
  closeEvent.emit();
}

void Window::show() noexcept {
  nativeWindow->show();
}

void Window::hide() noexcept {
  nativeWindow->hide();
}

void Window::maximize() noexcept {
  nativeWindow->maximize();
}

void Window::minimize() noexcept {
  nativeWindow->minimize();
}

void Window::restoreDefaultSize() noexcept {
  nativeWindow->restoreDefaultSize();
}

void Window::adjustBounds(const Rectangle<int>& rect) noexcept {
  nativeWindow->adjustBounds(rect);
}

[[nodiscard]] Rectangle<int> Window::clientAreaBounds() const noexcept {
  return nativeWindow->clientAreaBounds();
}

[[nodiscard]] int Window::dpi() const noexcept {
  return nativeWindow->dpi();
}

[[nodiscard]] bool Window::handleCloseEvent() {
  closeEvent.emit();
  return false;
}

[[nodiscard]] bool Window::handleSizeChangedEvent(
    IntSize size, SizeChangedEvent::Type type) {
  boundsChangedEvent.emit(
      std::make_shared<SizeChangedEvent>(std::move(type), std::move(size)));
  return false;
}

}  // namespace idafucker