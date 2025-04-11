#pragma once
#include <WebView2.h>
#include <wrl.h>

#include <hyperui/CoreDefines.hpp>
#include <hyperui/engine/IRenderingEngine.hpp>

namespace wrl = Microsoft::WRL;

HYPERUI_NAMESPACE_BEGIN

struct WebView2RuntimeData final {
  constexpr WebView2RuntimeData() noexcept = default;

  wrl::ComPtr<ICoreWebView2Environment> env{};
  wrl::ComPtr<ICoreWebView2Controller> controller{};
  wrl::ComPtr<ICoreWebView2> webview{};
};

[[nodiscard]] auto make_webview2()

    class WebViewEngine : public IRenderingEngine {
  auto on_controller_created() {}

  // A callback to be called when a webview environment is created.
  auto on_env_created([[maybe_unused]] HRESULT result,
                      ICoreWebView2Environment *env) -> HRESULT {
    m_env = env;

    // Request controller creation.
    env->CreateCoreWebView2Controller(
        m_window->handle<HWND>(),
        wrl::Callback<
            ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            []() -> HRESULT {})
            .Get());
  }

 public:
  explicit WebViewEngine(const std::shared_ptr<idafucker::IWindow> &window)
      : m_window{window} {}

 private:
  std::shared_ptr<idafucker::IWindow> m_window;
  wrl::ComPtr<ICoreWebView2Environment> m_env;
};

HYPERUI_NAMESPACE_END