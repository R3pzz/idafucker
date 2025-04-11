#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/math/Common.hpp>

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

  [[nodiscard]] void* platformHandle() const noexcept;

 protected:
  virtual LRESULT onWindowMesasge(UINT message, WPARAM wparam, LPARAM lparam);

  HWND handle_;

  friend class ApplicationImpl;
};
}  // namespace win

IDAFUCKER_NAMESPACE_END