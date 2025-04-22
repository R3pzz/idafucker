#include <hyperui/runtime/impl/win/EngineImpl.hpp>
#include <idafucker/base/String.hpp>
#include <idafucker/exceptions/PlatformException.hpp>

#include <spdlog/spdlog.h>

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{
EngineImpl::EngineImpl(
    HWND window, const std::filesystem::path &userDataFolder)
    : window_{window}
{
  using Env = ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler;
  using Ctrl = ICoreWebView2CreateCoreWebView2ControllerCompletedHandler;

  std::atomic_bool done{false};

  const auto onCtrlCreated = [&](HRESULT code,
                                 ICoreWebView2Controller *ctrl) -> HRESULT {
    if (code != S_OK)
      throw idafucker::PlatformException{"WebView2 controller creation failed"};

    controller_ = ctrl;
    controller_->get_CoreWebView2(&core_);
    controller_->put_IsVisible(TRUE);

    done = true;
    return S_OK;
  };

  const auto onEnvCreated = [&](HRESULT code,
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
  pumpWebView2EventsUntil([&done] { return done.load(); });
}

void EngineImpl::navigate(const Url &where) noexcept
{
  core_->Navigate(where.c_str());
}

void EngineImpl::navigate(const HtmlFile &html) noexcept
{
  auto wideHtml = idafucker::convertToUnicode(html.data());
  core_->NavigateToString(wideHtml.c_str());
}

void EngineImpl::postMessage(const std::wstring &message) const noexcept
{
  core_->PostWebMessageAsJson(message.c_str());
}

void EngineImpl::addInitScript(const std::wstring &script) noexcept
{
  std::atomic_bool done{};

  core_->AddScriptToExecuteOnDocumentCreated(
      script.c_str(),
      makeHander<handlers::InitScriptAdded>(
          [&done]([[maybe_unused]] HRESULT code, LPCWSTR result) -> HRESULT {
            done = true;

            if (code != S_OK) {
              spdlog::warn(
                  "EngineImpl::addInitScript: failed to add an init "
                  "script:\n`{}`",
                  idafucker::convertToUtf8(result));
              return S_OK;
            }

            spdlog::debug(
                "EngineImpl::addInitScript: script added with `{}` result",
                idafucker::convertToUtf8(result));
            return S_OK;
          })
          .Get());

  // Unfortunately, we have to pump the message queue
  pumpWebView2EventsUntil([&done] { return done.load(); });
}

void EngineImpl::onResize(const RECT &size) noexcept
{
  controller_->put_Bounds(size);
}
}  // namespace impl::win

HYPERUI_NAMESPACE_END