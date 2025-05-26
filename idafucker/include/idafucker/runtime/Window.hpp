#pragma once
#include <memory> // unique_ptr

#include <fuse/Class.hpp>
#include <fuse/Signal.hpp>

#include <idafucker/runtime/Application.hpp>
#include <idafucker/runtime/WindowOptions.hpp>
#include <idafucker/runtime/events/Events.hpp>

// Event bus outline:
// Window: |
//         |     Base/derived
//         |    event handlers
// --------+-----------------------
// App:    |          |
//         |    OS-to-platform
//         |   event translation
// --------+-----------------------
// OS:     |          |
//         |     OS-specific
//         |    window events

namespace idafucker
{
class Window {
public:
  explicit Window(const Application& application, const WindowOptions& options);
  virtual ~Window() noexcept;

  // Window visibility operations.
  virtual void show() noexcept;
  virtual void hide() noexcept;

  // Window size/position operations.
  virtual void maximize() noexcept;
  virtual void minimize() noexcept;
  virtual void restoreDefaultSize() noexcept;
  virtual void adjustBounds(const Rectangle<int>& rect) noexcept;
  [[nodiscard]] virtual Rectangle<int> clientAreaBounds() const noexcept;

  // DPI support
  [[nodiscard]] virtual int dpi() const noexcept;

  // Events:
  fuse::Signal<void(BoundsChangedEvent::Ref)>
      boundsChangedEvent{};  //< The window bounds got changed.
  fuse::Signal<void(KeyboardEvent::Ref)>
      keyboardEvent{};  //< A key has been pressed/released on the keyboard.
  fuse::Signal<void(std::uint64_t)>
      mouseEvent{};  //< The mouse was moved/its key was pressed/released.
  fuse::Signal<void()> closeEvent{};  //< The window is preparing to be closed.
  fuse::Signal<void(std::uint64_t)> focusEvent{};  //< The window entered focus.

protected:
  [[nodiscard]] virtual bool handleCloseEvent();
  [[nodiscard]] virtual bool handleSizeChangedEvent(
      IntSize size, SizeChangedEvent::Type type);

  std::unique_ptr<detail::NativeWindow> nativeWindow;
private:

  friend class detail::NativeApplication;
  friend class detail::NativeWindow;

  FUSE_NONCOPYABLE(Window);
};
} // namespace idafucker