#pragma once
#include <filesystem>  // path
#include <memory>      // shared_ptr

#include <hyperui/CoreDefines.hpp>
#include <hyperui/data/HtmlFile.hpp>
#include <idafucker/exceptions/PlatformException.hpp>
#include <idafucker/runtime2/impl/win/Types.hpp>

#include "IncludeWebView2.hpp"

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{
// Encapsulates a WebView2 engine
class EngineImpl {
public:
  using Ref = std::shared_ptr<EngineImpl>;

  using Url = std::wstring;

  EngineImpl(HWND window, const std::filesystem::path &userDataFolde);

  // Navigation
  void navigate(const Url &where) noexcept;
  void navigate(const HtmlFile &html) noexcept;

  // Messaging
  void postMessage(const std::wstring &message) const noexcept;

  void addMessageListener(auto &&callback) noexcept
  {
    core_->add_WebMessageReceived(
        makeHander<handlers::MessageReceived>(
            std::forward<decltype(callback)>(callback))
            .Get(),
        nullptr);
  }

  // Events
  void addHotkeyListener(auto &&callback) noexcept
  {
    controller_->add_AcceleratorKeyPressed(
        makeHander<handlers::HotkeyPressed>(
            std::forward<decltype(callback)>(callback))
            .Get(),
        nullptr);
  }

  // Scripts
  void addInitScript(const std::wstring &script) noexcept;

  // Window events
  void onResize(const RECT &size) noexcept;

private:
  void pumpWebView2EventsUntil(auto &&function) noexcept
  {
    MSG message{};
    while (!function() && ::GetMessage(&message, NULL, 0u, 0u) >= 0) {
      ::TranslateMessage(&message);
      ::DispatchMessage(&message);
    }
  }

  HRESULT onEnvCreated(HRESULT code, ICoreWebView2Environment *env);
  HRESULT onCtrlCreated(HRESULT code, ICoreWebView2Controller *ctrl);

  HWND window_{};

  // WebView2 core
  ComPtr<ICoreWebView2> core_{};
  ComPtr<ICoreWebView2Controller> controller_{};
  ComPtr<ICoreWebView2Environment> env_{};
};
}  // namespace impl::win

HYPERUI_NAMESPACE_END