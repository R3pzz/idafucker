#include <objbase.h>
#include <sstream>  // stringstream
#include <wrl.h>

#include <idafucker/base/String.hpp>
#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime2/Application.hpp>
#include <idafucker/runtime2/Window.hpp>
#include <idafucker/runtime2/WindowOptions.hpp>
#include <hyperui/HyperuiWindow.hpp>
#include <hyperui/data/factories/HtmlFileFactory.hpp>

#include <spdlog/spdlog.h>

int main(int argc, char* argv[])
{
  using namespace idafucker;
  using namespace hyperui;

  try {
    spdlog::set_level(spdlog::level::debug);

    // Instantiate the application
    Application app{};

    // Instantiate the resource manager and the resource factories
    ResourceManager resourceManager{};
    resourceManager.pushFactory(".html", makeHtmlFileFactory());

    // Instantiate the main window
    WindowOptions options{.atom = app.wcAtom(),
                          .title = L"idafucker | x86_64 | v1.0.0",
                          .size = {1280, 960}};
    HyperuiWindow hyperuiWindow{options, resourceManager};

    // Put some event handlers
    hyperuiWindow.closeEvent << [&app]() -> void {
      spdlog::info("Main window closed. Terminating the application");
      app.terminate();
    };

    hyperuiWindow.show();

    // Load index.html from C:/boostware/
    auto index = resourceManager.load(L"C:/boostware/index.html");
    if (index == nullptr) {
      throw Exception{"index.html not found"};
    }

    auto data = index->get<HtmlFile>();
    hyperuiWindow.engine()->navigate(*data);

    app.runEventLoop();
  } catch (std::exception& e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}