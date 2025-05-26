#pragma once
#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <idafucker/Config.hpp>
#include <idafucker/runtime/Window.hpp>
#include <idafucker/runtime/Application.hpp>
#include <idafucker/runtime/WindowOptions.hpp>

#include <fuse/CommandLine.hpp>

namespace idafucker::detail
{
class NativeWindow final {
public:
  explicit NativeWindow(
      const Application& application, const WindowOptions& options,
      Window* wrapper);
  ~NativeWindow() noexcept;

  void show() noexcept;
  void hide() noexcept;
  void maximize() noexcept;
  void minimize() noexcept;
  void restoreDefaultSize() noexcept;
  void adjustBounds(const Rectangle<int>& rect) noexcept;
  [[nodiscard]] Rectangle<int> clientAreaBounds() const noexcept;

  [[nodiscard]] int dpi() const noexcept;
  [[nodiscard]] constexpr HWND handle() const noexcept {
    return handle_;
  }

private:
  HWND handle_;

  // clang-format off
  static LRESULT CALLBACK nativeMessageHandler(
      HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
  // clang-format on

  friend class NativeApplication;

  FUSE_NONCOPYABLE(NativeWindow);
};
}  // namespace idafucker::detail