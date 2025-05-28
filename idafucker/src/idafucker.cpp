#include <hyperui.hpp>

#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime/Application.hpp>

#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
  using namespace idafucker;

  try {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Entered in function");

    Application app{fuse::systemCommandLine()};

    // Initialize the resource manager
    ResourceManager resourceManager{};
    resourceManager.registerFactory(".html", hyperui::Markup::Factory::make());

    // Create a sample window
    hyperui::Window::Options options{};
    options.title = L"idafucker | v0.1-a | Windows";
    options.size = {1200, 1600};
    options.flags = hyperui::Window::Options::Flags::HasFixedSize;

    hyperui::Window window{app, options};
    window.closeEvent.connect([&] { app.terminate(); });
    window.boundsChangedEvent.connect([&](BoundsChangedEvent::Ref e) {
      auto sizeChangedEvent = std::static_pointer_cast<SizeChangedEvent>(e);
      if (sizeChangedEvent != nullptr) {
        const auto [w, h] = sizeChangedEvent->size();
        spdlog::debug("boundsChangedEvent: window size changed to {}x{}", w, h);
      }
    });

    hyperui::Bridge bridge{window.engine()};
    bridge.bindJSToNative("add", [](const nlohmann::json& args) -> std::string {
      return std::to_string(args[0].get<int>() + args[1].get<int>());
    });

    auto bindExample = resourceManager.load(
        "C:/Users/Murzila/source/repos/idafucker/examples/ui/bind/index.html",
        Resource::Flags::Required);
    window.engine().loadFromMarkup(*bindExample->get<hyperui::Markup>());
    window.show();

    // Run the event loop
    app.runEventLoop([&] { resourceManager.observe(); });
  } catch (std::exception& e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}