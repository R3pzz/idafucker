#pragma once
#include <introspect/Config.hpp>
#include <introspect/runtime/Application.hpp>
#include <introspect/runtime/Window.hpp>
#include <fuse/CommandLine.hpp>
#include <fuse/Platform.hpp>

namespace introspect::detail
{

class NativeWindow final {
public:
  explicit NativeWindow(const Application& application,
                        const Window::Options& options,
                        Window* wrapper);
  ~NativeWindow() noexcept;

  void show() noexcept;
  void hide() noexcept;
  void maximize() noexcept;
  void minimize() noexcept;
  void restoreDefaultSize() noexcept;
  void adjustBounds(const fuse::IntRectangle& rect) noexcept;
  [[nodiscard]]fuse::IntRectangle clientAreaBounds() const noexcept;
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

}  // namespace introspect::detail