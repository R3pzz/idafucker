#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <hyperui/Window.hpp>

#if FUSE_PLATFORM_WIN
  #include "../../../idafucker/src/win/runtime/NativeWindow.hpp"
  #include "../win/NativeEngine.hpp"
#elif FUSE_PLATFORM_LINUX
  #error "Not supported yet..."
#endif

namespace hyperui
{

Window::Window(const idafucker::Application &app, const Options &options)
    : idafucker::Window{app, options},
      engine_{std::make_unique<detail::NativeEngine>(native().handle(),
                                                     options.userDataFolder)} {}

[[nodiscard]] bool Window::handleSizeChangedEvent(
    idafucker::IntSize size,
    idafucker::SizeChangedEvent::Type type) {
  engine_->handleResizeEvent(clientAreaBounds());
  return idafucker::Window::handleSizeChangedEvent(size, type);
}

}  // namespace hyperui