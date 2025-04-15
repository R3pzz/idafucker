#include <hyperui/HyperuiWindow.hpp>
#include <hyperui/data/HtmlFile.hpp>

HYPERUI_NAMESPACE_BEGIN

LRESULT HyperuiWindow::onWindowMesasge(UINT message, WPARAM wparam,
                                       LPARAM lparam)
{
  switch (message) {
    case WM_SIZE: {
      RECT clientRect{};
      ::GetClientRect(handle_, &clientRect);
      engine_->onResize(clientRect);
    } break;
    default:
      break;
  }

  // Preserve the native message processing
  return idafucker::Window::onWindowMesasge(message, wparam, lparam);
}

HYPERUI_NAMESPACE_END