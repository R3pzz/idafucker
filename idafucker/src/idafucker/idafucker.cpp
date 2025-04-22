#include <objbase.h>
#include <sstream>  // stringstream
#include <wrl.h>

#include <hyperui/HyperuiWindow.hpp>
#include <hyperui/data/factories/HtmlFileFactory.hpp>
#include <hyperui/rpc/Host.hpp>
#include <idafucker/base/String.hpp>
#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime2/Application.hpp>
#include <idafucker/runtime2/Window.hpp>
#include <idafucker/runtime2/WindowOptions.hpp>

#include <spdlog/spdlog.h>

constexpr auto SampleHtmlPath{
    "../../../idafucker/examples/hyperui/bind/index.html"};

int main(int argc, char* argv[])
{
  using namespace idafucker;
  using namespace hyperui;

  try {
    spdlog::set_level(spdlog::level::debug);

    // Instantiate the command line parser object
    CommandLine commandLine{::GetCommandLine()};

    // Instantiate the application given the command line
    Application app{commandLine};

    // Instantiate the resource manager and the resource factories
    ResourceManager resourceManager{};
    resourceManager.registerFactory(".html", makeHtmlFileFactory());

    // Instantiate the main window
    HyperuiWindowOptions options{};
    options.atom = app.wcAtom(), options.title = L"idafucker | x86_64 | v1.0.0",
    options.size = {1280, 960};
    options.hotkeysToDisable |= HyperuiWindowOptions::Hotkeys::F5;
    HyperuiWindow hyperuiWindow{options, commandLine};

    // Add an on-close event handler
    hyperuiWindow.closeEvent << [&app]() -> void {
      spdlog::info("Main window closed. Terminating the application");
      app.terminate();
    };

    // Show the main window
    hyperuiWindow.show();

    // Disable the context menu
    hyperuiWindow.engine()->addInitScript(
        LR"js(
document.addEventListener('DOMContentLoaded', () => {
  document.addEventListener('contextmenu', (e) => {
    e.preventDefault()
  })
})
    )js");

    // Instantiate the js-native RPC
    RpcHost rpcHost{*hyperuiWindow.engine()};

    // Bind the `add` function that the javascript code calls in
    // `await window.nativeBridge.add(a, b)` in the `index.html` file
    rpcHost.bind("add", [](const nlohmann::json& args) -> std::string {
      return std::to_string(args[0].get<int>() + args[1].get<int>());
    });

    // Load the `index.html` file from
    // `idafucker/idafucker/examples/hyperui/bind/`
    auto index = resourceManager.load(SampleHtmlPath, {});
    if (index == nullptr)
      throw Exception{"index.html not found"};

    // Add a hot-reload hotkey
    hyperuiWindow.keyboardEvent << [&](KeyboardEvent::Ref event) -> void {
      if (event->state() == KeyboardEvent::State::Pressed &&
          event->virtualKeyCode() == VK_F5) {
        resourceManager.reload(index);  //< Reload the underlying web resource
        hyperuiWindow.engine()->navigate(
            *index->get<HtmlFile>());  //< Re-open the newly loaded html file
      }
    };

    // Open up the `index.html` file
    hyperuiWindow.engine()->navigate(*index->get<HtmlFile>());

    // Run the event loop
    app.runEventLoop([&] { resourceManager.observe(); });
  } catch (std::exception& e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}