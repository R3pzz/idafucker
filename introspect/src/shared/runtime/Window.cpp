#include <introspect/runtime/Window.hpp>

#if FUSE_PLATFORM_WIN
  #include "../../win/runtime/NativeWindow.hpp"
#elif FUSE_PLATFORM_LINUX
  #error "Not supported yet..."
#endif

namespace introspect
{

Window::Window(const Application& application, const Options& options)
    : native_{new detail::NativeWindow{application, options, this}} {}

Window::~Window() {
  closeEvent.emit();
}

void Window::show() noexcept {
  native_->show();
}

void Window::hide() noexcept {
  native_->hide();
}

void Window::maximize() noexcept {
  native_->maximize();
}

void Window::minimize() noexcept {
  native_->minimize();
}

void Window::restoreDefaultSize() noexcept {
  native_->restoreDefaultSize();
}

void Window::adjustBounds(const fuse::IntRectangle& rect) noexcept {
  native_->adjustBounds(rect);
}

[[nodiscard]] fuse::IntRectangle Window::clientAreaBounds() const noexcept {
  return native_->clientAreaBounds();
}

[[nodiscard]] int Window::dpi() const noexcept {
  return native_->dpi();
}

[[nodiscard]] bool Window::handleCloseEvent() {
  closeEvent.emit();
  return false;
}

[[nodiscard]] bool Window::handleSizeChangedEvent(fuse::Vector2i size,
                                                  SizeChangedEvent::Type type) {
  boundsChangedEvent.emit(
      std::make_shared<SizeChangedEvent>(std::move(type), std::move(size)));
  return false;
}

}  // namespace introspect