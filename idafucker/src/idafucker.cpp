#include <bridge.hpp>
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
    hyperui::Bridge bridge{window.engine()};

    window.closeEvent.connect([&] { app.terminate(); });
    window.boundsChangedEvent.connect([&](BoundsChangedEvent::Ref e) {
      auto sizeChangedEvent = std::static_pointer_cast<SizeChangedEvent>(e);
      if (sizeChangedEvent != nullptr) {
        const auto [w, h] = sizeChangedEvent->size();
        spdlog::debug("boundsChangedEvent: window size changed to {}x{}", w, h);
      }
    });

    bridge::Process process{};

    bridge.bindJSToNative("attachToProcess", [&process](const nlohmann::json& args) -> std::string {
      const std::string &processName = args.at(0u);
      bridge::ProcessRegistry::processByName(fuse::underlyingCast(processName), bridge::Access::AllAccess);
    });

    auto bindExample = resourceManager.load(
        "C:/Users/Murzila/source/repos/idafucker/examples/ui/bind/index.html",
        Resource::Flags::Required);
    window.engine().loadFromMarkup(*bindExample->get<hyperui::Markup>());
    window.show();

    auto chromeProcess = bridge::win::ProcessRegistry::processByName(
        L"chrome.exe", bridge::win::Process::Access::AllAccess);

    chromeProcess.and_then([](bridge::win::Process& process)
                               -> bridge::Result<bridge::win::Process> {
      process.isDebuggerAttached()
          .and_then([](bool value) -> bridge::Result<bool> {
            spdlog::info("Being debugged: {}", value);
            return value;
          })
          .or_else([](bridge::Status code) -> bridge::Result<bool> {
            throw std::runtime_error{code.string()};
            return false;
          });

      return {};
    });

    // Run the event loop
    app.runEventLoop([&] { resourceManager.observe(); });
  } catch (std::exception& e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}