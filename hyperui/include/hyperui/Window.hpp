#pragma once
#include <filesystem> // path

#include <hyperui/Config.hpp>
#include <hyperui/Engine.hpp>
#include <idafucker/runtime/Window.hpp>

namespace hyperui
{
// A `Window` extension that is built to handle WebView2 engine events
class Window final : public idafucker::Window {
public:
  using Ref = std::shared_ptr<Window>;

  struct Options : idafucker::WindowOptions {
    static constexpr auto kDefaultUDF{L"C:/temp/idafucker/hyperui/"};

    std::wstring userDataFolder{kDefaultUDF};
  };

  explicit Window(const idafucker::Application &app, const Options &options);

  [[nodiscard]] constexpr Engine &engine() noexcept {
    return *engine_;
  }

  [[nodiscard]] constexpr const Engine &engine() const noexcept {
    return *engine_;
  }

protected:
  [[nodiscard]] bool handleSizeChangedEvent(
      idafucker::IntSize size, idafucker::SizeChangedEvent::Type type) override;

private:
  std::unique_ptr<Engine> engine_;
};
}  // namespace hyperui