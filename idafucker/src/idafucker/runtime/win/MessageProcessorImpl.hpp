#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/MessageProcessor.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace win {
class MessageProcessorImpl final : public MessageProcessor {
  IDAFUCKER_NONCOPYABLE(MessageProcessorImpl);

 public:
  constexpr MessageProcessorImpl() noexcept = default;

  // Called when a window is unregistered from the application.
  ~MessageProcessorImpl() noexcept override;

  [[nodiscard]] LRESULT onWindowProcess(HWND window, UINT message,
                                        WPARAM wparam, LPARAM lparam) noexcept;

  static LRESULT CALLBACK windowProcess(HWND window, UINT message,
                                        WPARAM wparam, LPARAM lparam);
};
}  // namespace win

IDAFUCKER_NAMESPACE_END