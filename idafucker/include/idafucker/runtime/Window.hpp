#pragma once
#include <memory>  // weak_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/math/Common.hpp>

#include "MessageProcessor.hpp"
#include "WindowSpecs.hpp"

IDAFUCKER_NAMESPACE_BEGIN

constexpr auto unchanged{-1};

class Window {
 public:
  using Ref = std::shared_ptr<Window>;

  virtual ~Window() noexcept = default;

  // Window visibility operations.
  virtual void show() noexcept = 0;
  virtual void hide() noexcept = 0;

  // Window size/position operations.
  virtual void maximize() noexcept = 0;
  virtual void minimize() noexcept = 0;

  virtual void restoreDefaultSize() noexcept = 0;
  virtual void adjustBounds(const Rectangle<int>& rect) noexcept = 0;

  void resizeTo(const Size& size) noexcept
  {
    adjustBounds({unchanged, unchanged, size.x, size.y});
  }

  void moveTo(const Position& position) noexcept
  {
    adjustBounds({position.x, position.y, unchanged, unchanged});
  }

  // DPI support
  [[nodiscard]] virtual bool isDpiSupported() const noexcept = 0;
  [[nodiscard]] virtual int dpi() const noexcept = 0;

  // Messages
  [[nodiscard]] virtual MessageProcessor& messageProcessor() noexcept = 0;

 private:
  [[nodiscard]] virtual void* platformHandle() const noexcept = 0;

 public:
#if IDAFUCKER_PLATFORM_WIN32
  [[nodiscard]] HWND handle() const noexcept
  {
    return reinterpret_cast<HWND>(platformHandle());
  }
#elif IDAFUCKER_PLATFORM_LINUX
  #error "Platform not supported"
#else
  #error "Platform not supported"
#endif
};

IDAFUCKER_NAMESPACE_END