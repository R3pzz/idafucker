#pragma once
#include <hyperui/CoreDefines.hpp>
#include <hyperui/runtime/Engine.hpp>
#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime2/Window.hpp>
#include <idafucker/runtime2/WindowOptions.hpp>

HYPERUI_NAMESPACE_BEGIN

class HyperuiWindow final : public idafucker::Window {
 public:
  static constexpr auto UserDataFolder{L"C:/boostware/"};

  HyperuiWindow(const idafucker::WindowOptions &options,
                idafucker::ResourceManager &resourceManager)
      : idafucker::Window{options},
        resourceManager_{resourceManager},
        engine_{new Engine{handle_, UserDataFolder}}
  {
  }

  [[nodiscard]] constexpr auto &engine() noexcept
  {
    return engine_;
  }

 private:
  // Custom window message handler
  LRESULT onWindowMesasge(UINT message, WPARAM wparam, LPARAM lparam) override;

  idafucker::ResourceManager &resourceManager_;
  std::vector<idafucker::IntrusiveHandle<idafucker::Resource>> resources_{};
  std::shared_ptr<Engine> engine_;
};

HYPERUI_NAMESPACE_END