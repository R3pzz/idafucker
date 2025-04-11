#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/Window.hpp>
#include <idafucker/runtime/WindowSpecs.hpp>

#include "MessageProcessorImpl.hpp"

#include <spdlog/spdlog.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace win {
class WindowImpl final : public Window {
  IDAFUCKER_NONCOPYABLE(WindowImpl);

 public:
  WindowImpl(const WindowSpecs& specs, ATOM atom);
  ~WindowImpl() noexcept override;

  // Window visibility operations.
  void show() noexcept override;
  void hide() noexcept override;

  // Window size/position operations.
  void maximize() noexcept override;
  void minimize() noexcept override;

  void restoreDefaultSize() noexcept override;
  void adjustBounds(const Rectangle<int>& rect) noexcept override;

  // DPI support
  [[nodiscard]] bool isDpiSupported() const noexcept override;
  [[nodiscard]] int dpi() const noexcept override;

  // Messages
  [[nodiscard]] MessageProcessor& messageProcessor() noexcept override;

 private:
  [[nodiscard]] void* platformHandle() const noexcept override;

  // DPI management
  void configureDpi();

  HWND handle_;
  MessageProcessorImpl messageProcessor_;
};
}  // namespace win

IDAFUCKER_NAMESPACE_END