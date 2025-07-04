#pragma once
#include <filesystem>  // path

#include <hyperui/Config.hpp>
#include <hyperui/Engine.hpp>
#include <introspect/runtime/Window.hpp>

namespace hyperui
{

// A `Window` extension that is built to handle WebView2 engine events
class Window final : public introspect::Window {
public:
  using Ref = std::shared_ptr<Window>;

  struct Options : introspect::Window::Options {
    static constexpr auto k_defaultUDF{L"C:/temp/introspect/hyperui/"};

    std::wstring userDataFolder{k_defaultUDF};
  };

  explicit Window(const introspect::Application &app, const Options &options);

  [[nodiscard]] constexpr Engine &engine() noexcept {
    return *engine_;
  }

  [[nodiscard]] constexpr const Engine &engine() const noexcept {
    return *engine_;
  }

protected:
  [[nodiscard]] bool handleSizeChangedEvent(
      fuse::Vector2i size,
      introspect::SizeChangedEvent::Type type) override;

private:
  std::unique_ptr<Engine> engine_;
};

}  // namespace hyperui