#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime/Application.hpp>
#include <hyperui.hpp>

#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
  using namespace idafucker;

  try {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Entered in function");

    Application app{fuse::systemCommandLine()};

    // Initialize the resource manager
    ResourceManager resourceManager{};
    
    // Create a sample window
    hyperui::Window::Options options{};
    options.title = L"idafucker | v0.1-a | Windows";
    options.size = {800, 600};

    hyperui::Window window{app, options};
    window.closeEvent.connect([&] { app.terminate(); });
    window.boundsChangedEvent.connect([&](BoundsChangedEvent::Ref e) {
      auto sizeChangedEvent = std::static_pointer_cast<SizeChangedEvent>(e);
      if (sizeChangedEvent != nullptr) {
        const auto [w, h] = sizeChangedEvent->size();
        spdlog::debug("boundsChangedEvent: window size changed to {}x{}", w, h);
      }
    });

    window.engine().loadFromURL(L"https://www.google.com/");
    window.show();

    // Run the event loop
    app.runEventLoop([&] { resourceManager.observe(); });
  } catch (std::exception& e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}