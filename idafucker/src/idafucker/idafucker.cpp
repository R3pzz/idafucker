#include <objbase.h>
#include <sstream>  // stringstream
#include <wrl.h>

#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/runtime/ApplicationFactory.hpp>
#include <idafucker/runtime/messages/CloseMessage.hpp>

#include <sciter-js/sciter-x-api.h>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[])
{
  using namespace idafucker;

  try {
    spdlog::set_level(spdlog::level::debug);

    auto app = ApplicationFactory::create(argc, argv);
    app->onNewWindow << [](Window::Ref window) noexcept -> void {
      spdlog::info("Window created with dpi {}", window->dpi());
    };

    auto window =
        app->makeWindow({"IDAFUCKER", {800, 600}, WindowType::Default, true});
    
    window->messageProcessor().onMessage
        << [app](Message::Ref message) -> void {
      if (auto closeMessage = std::dynamic_pointer_cast<CloseMessage>(message);
          closeMessage != nullptr) {
        spdlog::info(
            "CloseMessage received. Terminating the "
            "application");
        app->terminate();
      }
    };
    window->show();

    while (app->running()) {
      MSG msg{};

      while (::PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
      }
    }
  } catch (std::exception& e) {
    spdlog::critical("Exception caught: {}", e.what());
    std::this_thread::sleep_for(std::chrono::seconds{10u});
  }

  return 0;
}