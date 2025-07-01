#include <hyperui/Window.hpp>

#if FUSE_PLATFORM_WIN
  #include "../../../introspect/src/win/runtime/NativeWindow.hpp"
  #include "../win/NativeEngine.hpp"
#elif FUSE_PLATFORM_LINUX
  #error "Not supported yet..."
#endif

namespace hyperui
{

Window::Window(const introspect::Application &app, const Options &options)
    : introspect::Window{app, options},
      engine_{std::make_unique<detail::NativeEngine>(native().handle(),
                                                     options.userDataFolder)} {}

[[nodiscard]] bool Window::handleSizeChangedEvent(
    introspect::IntSize size,
    introspect::SizeChangedEvent::Type type) {
  engine_->handleResizeEvent(clientAreaBounds());
  return introspect::Window::handleSizeChangedEvent(size, type);
}

}  // namespace hyperui