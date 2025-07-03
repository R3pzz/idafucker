#pragma once
#include <memory> // unique_ptr

#include <fuse/Bits.hpp>
#include <fuse/Class.hpp>
#include <fuse/Signal.hpp>
#include <fuse/math/Rectangle.hpp>

#include <introspect/runtime/Application.hpp>
#include <introspect/runtime/events/Events.hpp>

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

namespace introspect
{
  
class Window {
public:
  struct Options {
    enum class Flags : std::uint32_t {
      None = 0u,

      // Decorations:
      HasFixedSize = 1u << 0u, //< This window is not resizable by the user.
      HasCustomTitleBar = 1u << 1u, //< Ask system to remove the default title bar.
      
      // Window types:
      IsPopup = 1u << 2u, //< This window is a popup window.
    };

    [[nodiscard]] constexpr bool hasFixedSize() const noexcept {
      return flags.contains(Flags::HasFixedSize);
    }
    
    [[nodiscard]] constexpr bool hasCustomTitleBar() const noexcept {
      return flags.contains(Flags::HasCustomTitleBar);
    }

    [[nodiscard]] constexpr bool isDefault() const noexcept {
      return !flags.contains(Flags::IsPopup);
    }
    
    [[nodiscard]] constexpr bool isPopup() const noexcept {
      return flags.contains(Flags::IsPopup);
    }

    std::wstring title{};
    fuse::Vector2i size{};
    fuse::Bits<Flags> flags{};
  };

  explicit Window(const Application& application, const Options& options);
  virtual ~Window() noexcept;

  // Window visibility operations.
  virtual void show() noexcept;
  virtual void hide() noexcept;

  // Window size/position operations.
  virtual void maximize() noexcept;
  virtual void minimize() noexcept;
  virtual void restoreDefaultSize() noexcept;
  virtual void adjustBounds(const fuse::IntRectangle& rect) noexcept;
  [[nodiscard]] virtual fuse::IntRectangle clientAreaBounds() const noexcept;

  // DPI support.
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
      fuse::Vector2i size, SizeChangedEvent::Type type);

  // Native is exposed for derived classes so that they can make custom calls
  // to the native API.
  [[nodiscard]] constexpr auto &native() noexcept { return *native_; }
  [[nodiscard]] constexpr const auto &native() const noexcept { return *native_; }

private:
  std::unique_ptr<detail::NativeWindow> native_;

  friend class detail::NativeApplication;
  friend class detail::NativeWindow;

  FUSE_NONCOPYABLE(Window);
};

} // namespace introspect