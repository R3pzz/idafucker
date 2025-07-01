#pragma once
#include <hyperui/Config.hpp>
#include <hyperui/Engine.hpp>
#include <hyperui/Window.hpp>

#include "COMHandlers.hpp"

namespace hyperui::detail
{

class NativeEngine final : public Engine {
public:
  NativeEngine(HWND window, const std::wstring &udf);

  void loadFromMarkup(const Markup &markup) override;
  void loadFromURL(const std::wstring &url) override;
  void addJSInitializationScript(const std::wstring &script) override;
  void postJSMessage(const std::wstring &message) noexcept override;
  void handleResizeEvent(const introspect::Rectangle<int> &bounds) override;

private:
  void pumpMessagesUntil(auto &&function) noexcept {
    MSG message{};
    while (!function() && ::GetMessageW(&message, NULL, 0u, 0u) >= 0) {
      ::TranslateMessage(&message);
      ::DispatchMessageW(&message);
    }
  }
  
  HWND window{};
  wrl::ComPtr<ICoreWebView2> core{};
  wrl::ComPtr<ICoreWebView2Controller> ctrl{};
  wrl::ComPtr<COMEventHandler> eventHandler{};
};

}  // namespace hyperui:::detail