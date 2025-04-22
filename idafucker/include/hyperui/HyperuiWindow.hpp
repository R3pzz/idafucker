#pragma once
#include <hyperui/CoreDefines.hpp>
#include <hyperui/runtime/Engine.hpp>
#include <idafucker/base/CommandLine.hpp>
#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime2/Window.hpp>

#include "HyperuiWindowOptions.hpp"

HYPERUI_NAMESPACE_BEGIN

// A `Window` extension that is built to handle WebView2 engine events
class HyperuiWindow final : public idafucker::Window {
 public:
  static constexpr auto DefaultUserDataFolder{L"C:/temp/idafucker/hyperui/"};

  HyperuiWindow(
      const HyperuiWindowOptions &options,
      const idafucker::CommandLine<idafucker::impl::win::Character>
          &commandLine);

  [[nodiscard]] constexpr auto &engine() noexcept
  {
    return engine_;
  }

 private:
  // Custom window message handler
  LRESULT onWindowMesasge(UINT message, WPARAM wparam, LPARAM lparam) override;

  Engine::Ref engine_;
};

HYPERUI_NAMESPACE_END