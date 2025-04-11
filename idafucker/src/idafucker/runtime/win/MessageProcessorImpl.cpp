#include <idafucker/runtime/messages/CloseMessage.hpp>

#include "MessageProcessorImpl.hpp"
#include "WindowImpl.hpp"

#include <sciter-js/sciter-x-api.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace win {
MessageProcessorImpl::~MessageProcessorImpl() noexcept {}

[[nodiscard]] LRESULT MessageProcessorImpl::onWindowProcess(
    HWND window, UINT message, WPARAM wparam, LPARAM lparam) noexcept
{
  onMessageLoop.emit();

  SBOOL handled{};
  ::SciterProcND(window, message, wparam, lparam, &handled);

  switch (message) {
    case WM_CLOSE:
      onMessage.emit(std::make_shared<CloseMessage>());
      return ::DefWindowProc(window, message, wparam, lparam);
    case WM_DESTROY:
      return ::DefWindowProc(window, message, wparam, lparam);
    default:
      return ::DefWindowProc(window, message, wparam, lparam);
  }

  return 0;
}

LRESULT CALLBACK MessageProcessorImpl::windowProcess(HWND window, UINT message,
                                                     WPARAM wparam,
                                                     LPARAM lparam)
{
  auto windowImpl =
      reinterpret_cast<WindowImpl *>(::GetWindowLongPtr(window, GWLP_USERDATA));
  if (windowImpl == nullptr) {
    return ::DefWindowProc(window, message, wparam, lparam);
  }

  auto processor =
      reinterpret_cast<MessageProcessorImpl *>(&windowImpl->messageProcessor());
  return processor->onWindowProcess(window, message, wparam, lparam);
}
}  // namespace win

IDAFUCKER_NAMESPACE_END