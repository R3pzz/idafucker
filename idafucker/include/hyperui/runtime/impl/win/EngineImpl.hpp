#pragma once
#include <filesystem>  // path
#include <memory>      // shared_ptr

#include <hyperui/CoreDefines.hpp>
#include <hyperui/data/HtmlFile.hpp>
#include <idafucker/runtime2/impl/win/Types.hpp>

#include "InteropHostImpl.hpp"

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{
// Encapsulates a WebView2 engine.
class EngineImpl {
 public:
  using Ref = std::shared_ptr<EngineImpl>;
  using Url = std::wstring;

  EngineImpl(HWND window, const std::filesystem::path &userDataFolder);

  // Interop
  [[nodiscard]] InteropHostImpl &interop() noexcept
  {
    return *interop_.Get();
  }

  // WebView2 navigation
  void navigate(const Url &where) noexcept;
  void navigate(const HtmlFile &html) noexcept;

  // Window events
  void onResize(const RECT &size) noexcept;

 private:
  HRESULT onEnvCreated(HRESULT code, ICoreWebView2Environment *env);
  HRESULT onCtrlCreated(HRESULT code, ICoreWebView2Controller *ctrl);

  HWND window_{};

  // WebView2 core
  ComPtr<ICoreWebView2> core_{};
  ComPtr<ICoreWebView2Controller> controller_{};
  ComPtr<ICoreWebView2Environment> env_{};
  
  // Web<->app comunication host
  ComPtr<InteropHostImpl> interop_{};
  EventRegistrationToken interopToken_{};
};
}  // namespace impl::win

HYPERUI_NAMESPACE_END