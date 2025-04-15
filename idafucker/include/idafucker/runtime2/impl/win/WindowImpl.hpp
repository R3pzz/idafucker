#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/Signal.hpp>
#include <idafucker/math/Common.hpp>
#include <idafucker/runtime2/events/Events.hpp>

#include "WindowOptionsImpl.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace impl::win {

// Just a wrapper around HWND.
// This class does not process any messages.
// This is essentially a base class for creating custom windows
// with custom message processors/callbacks/etc...
class WindowImpl {
  IDAFUCKER_NONCOPYABLE(WindowImpl);

 public:
  explicit WindowImpl(const WindowOptionsImpl& options);
  virtual ~WindowImpl() noexcept;

  // Window visibility operations.
  virtual void show() noexcept;
  virtual void hide() noexcept;

  // Window size/position operations.
  virtual void maximize() noexcept;
  virtual void minimize() noexcept;

  virtual void restoreDefaultSize() noexcept;
  virtual void adjustBounds(const Rectangle<int>& rect) noexcept;

  // DPI support
  [[nodiscard]] virtual int dpi() const noexcept;

  // Native interface
  [[nodiscard]] HWND platformHandle() const noexcept;

  // Events:
  Signal<void(BoundsChangedEvent::Ref)> boundsChangedEvent{};  //< The window bounds got changed.
  Signal<void(std::uint64_t)> keyboardEvent{}; //< A key has been pressed/released on the keyboard.
  Signal<void(std::uint64_t)> mouseEvent{}; //< The mouse was moved/its key was pressed/released.
  Signal<void()> closeEvent{}; //< The window is preparing to be closed.
  Signal<void(std::uint64_t)> focusEvent{}; //< The window entered focus.

 protected:
  virtual LRESULT onWindowMesasge(UINT message, WPARAM wparam, LPARAM lparam);

  HWND handle_;

  friend class ApplicationImpl;
};
}  // namespace win

IDAFUCKER_NAMESPACE_END