#define FUSE_EXPOSE_SYSTEM_HEADERS
#include "NativeEngine.hpp"

#include <fuse/String.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace hyperui::detail
{
NativeEngine::NativeEngine(HWND window, const std::wstring &udf)
    : window{window}, eventHandler{new COMEventHandler{}} {
  std::atomic_bool done{false};

  // The COM architecture implies that we allocate the objects in heap
  // and not on the stack. This means that this implementation is
  // technically bad and it is way better if we manage it using smart pointers.
  COMCreationHandler handler{window, [&](ICoreWebView2Controller *ctrl) {
                               if (ctrl != nullptr) {
                                 this->ctrl = ctrl;
                                 this->ctrl->get_CoreWebView2(&core);
                               }
                               done = true;
                             }};
  if (!handler.requestWebView2Creation(udf))
    throw std::runtime_error{"webview2 engine creation request failed"};

  // Pump webview2 message loop until the core is created
  pumpMessagesUntil([&done] { return done.load(); });

  // Get the WebView2 instance
  if (ctrl == nullptr)
    throw std::runtime_error{"webview2 controller creation failed"};
  ctrl->get_CoreWebView2(&core);

  // Install the message callback
  eventHandler->webMessageReceivedEvent.connect(
      [&](const std::wstring &message) {
        // We first need to convert the message into a UTF-8 string
        auto utf8Message = fuse::utf8Cast(message);
        jsMessageReceivedEvent.emit(
            nlohmann::json::parse(std::move(utf8Message)));
      });

  EventRegistrationToken token{};
  core->add_WebMessageReceived(eventHandler.Get(), &token);

  // After everything has been set up, show the WebView2 window
  ctrl->put_IsVisible(TRUE);
}

void NativeEngine::loadFromMarkup(const Markup &markup) {
  core->NavigateToString(markup.buffer().c_str());
}

void NativeEngine::loadFromURL(const std::wstring &url) {
  core->Navigate(url.c_str());
}

void NativeEngine::addJSInitializationScript(const std::wstring &script) {
  core->AddScriptToExecuteOnDocumentCreated(script.c_str(), eventHandler.Get());
}

void NativeEngine::postJSMessage(const std::wstring &message) noexcept {
  core->PostWebMessageAsJson(message.c_str());
}

void NativeEngine::handleResizeEvent(const idafucker::Rectangle<int> &bounds) {
  ctrl->put_Bounds(*reinterpret_cast<const RECT *>(&bounds));
}
}  // namespace hyperui::detail