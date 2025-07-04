#include <bridge.hpp>
#include <hyperui.hpp>

#include <introspect/resources/ResourceManager.hpp>
#include <introspect/runtime/Application.hpp>

#include <spdlog/spdlog.h>

// Define the minimal accepted logging level. It basically resembles the "priority" of a
// logged message. Priorities are sorted out from least important and most detailed -
// `trace` to more important and least detailed - `critical`.
#if _DEBUG
  // We would like to log out every step of the execution when debugging.
  #define INTROSPECT_SPDLOG_LEVEL (spdlog::level::trace)
#else
  // No need to pollute the console/logs with useless debugger info.
  #define INTROSPECT_SPDLOG_LEVEL (spdlog::level::info)
#endif

int main() {
  using namespace introspect;

  try {
    // Set the minimum logging level
    spdlog::set_level(INTROSPECT_SPDLOG_LEVEL);

    // Initialize the main runtime
    Application app{fuse::systemCommandLine()};

    // Initialize the resource manager
    ResourceManager resourceManager{};
    resourceManager.registerFactory(".html", hyperui::Markup::Factory::make());

    // Create a sample window
    hyperui::Window::Options options{};
    options.title = L"introspect | v0.1-a | Windows";
    options.size = {1200, 1600};
    options.flags = hyperui::Window::Options::Flags::HasFixedSize;

    // Set up the events
    hyperui::Window window{app, options};
    window.closeEvent.connect([&] { app.terminate(); });
    window.boundsChangedEvent.connect([&](BoundsChangedEvent::Ref e) {
      auto sizeChangedEvent = std::static_pointer_cast<SizeChangedEvent>(e);
      if (sizeChangedEvent != nullptr) {
        const auto [w, h] = sizeChangedEvent->size();
        spdlog::debug("boundsChangedEvent: window size changed to {}x{}", w, h);
      }
    });

    // Set up the RPC
    hyperui::Bridge bridge{window.engine()};
    bridge.bind("attach", [](const nlohmann::json &args) -> std::string {
      const std::string &name = args[0u];

      auto process = bridge::process::ProcessRegistry::find(fuse::unicodeCast(name),
                                                            false);
      if (!process.has_value()) {
        return process.error().explain();
      }

      const auto pid = process->pid();
      if (!pid.has_value()) {
        return pid.error().explain();
      }

      return std::to_string(pid.value());
    });

    auto bindExample = resourceManager.load(
        "C:/Users/Murzila/source/repos/idafucker/examples/ui/bind/index.html",
        Resource::Flags::Required);
    window.engine().loadFromMarkup(*bindExample->get<hyperui::Markup>());
    window.show();

    // Run the event loop
    app.runEventLoop([&] { resourceManager.observe(); });
  } catch (std::exception &e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}