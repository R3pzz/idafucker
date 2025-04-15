#include <hyperui/runtime/impl/win/EngineImpl.hpp>
#include <idafucker/base/String.hpp>
#include <idafucker/exceptions/PlatformException.hpp>

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{
EngineImpl::EngineImpl(HWND window, const std::filesystem::path &userDataFolder)
    : window_{window}, interop_{new InteropHostImpl{}}
{
  using Env = ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler;
  using Ctrl = ICoreWebView2CreateCoreWebView2ControllerCompletedHandler;

  std::atomic_bool initDone{false};

  auto onCtrlCreated = [&](HRESULT code,
                           ICoreWebView2Controller *ctrl) -> HRESULT {
    if (code != S_OK)
      throw idafucker::PlatformException{"WebView2 controller creation failed"};

    controller_ = ctrl;
    controller_->get_CoreWebView2(&core_);
    controller_->put_IsVisible(TRUE);

    core_->add_WebMessageReceived(interop_.Get(), &interopToken_);

    initDone = true;
    return S_OK;
  };

  auto onEnvCreated = [&](HRESULT code,
                          ICoreWebView2Environment *env) -> HRESULT {
    if (code != S_OK)
      throw idafucker::PlatformException{"WebView2 env creation failed"};

    env_ = env;
    env_->CreateCoreWebView2Controller(
        window_, Callback<Ctrl>(onCtrlCreated).Get());
    return S_OK;
  };

  ::CreateCoreWebView2EnvironmentWithOptions(
      nullptr, userDataFolder.c_str(), nullptr,
      Callback<Env>(onEnvCreated).Get());

  // Let webview2 process its messages
  MSG msg{};
  while (!initDone && ::GetMessage(&msg, nullptr, 0u, 0u) >= 0) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
}

void EngineImpl::navigate(const Url &where) noexcept
{
  core_->Navigate(where.c_str());
}

void EngineImpl::navigate(const HtmlFile &html) noexcept
{
  auto wideHtml = idafucker::widen(html.data());
  core_->NavigateToString(wideHtml.c_str());
}

void EngineImpl::onResize(const RECT &size) noexcept
{
  controller_->put_Bounds(size);
}
}  // namespace impl::win

HYPERUI_NAMESPACE_END