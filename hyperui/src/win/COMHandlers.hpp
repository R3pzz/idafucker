#pragma once
#include <atomic>      // atomic_size_t
#include <functional>  // function
#include <string>      // wstring

#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <hyperui/Config.hpp>
#include <hyperui/Window.hpp>

#include <fuse/Signal.hpp>

#include "WebViewHelpers.hpp"

namespace hyperui::detail
{
class COMCreationHandler final
    : public ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler,
      public ICoreWebView2CreateCoreWebView2ControllerCompletedHandler {
public:
  static constexpr auto kEnvCreatedIID{
      __uuidof(ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler)};
  static constexpr auto kCtrlCreatedIID{
      __uuidof(ICoreWebView2CreateCoreWebView2ControllerCompletedHandler)};

  constexpr COMCreationHandler(HWND window, auto &&completeHandler) noexcept
      : completeHandler{std::forward<decltype(completeHandler)>(
            completeHandler)},
        window{window} {}

  [[nodiscard]] bool requestWebView2Creation(const std::wstring &udf);

  HRESULT STDMETHODCALLTYPE
  QueryInterface(REFIID riid, void **ppvObject) override {
    if (ppvObject == nullptr)
      return E_INVALIDARG;

    if (::IsEqualIID(riid, kEnvCreatedIID) ||
        ::IsEqualIID(riid, kCtrlCreatedIID)) {
      *ppvObject = this;
      return S_OK;
    } else {
      return E_NOINTERFACE;
    }
  }

  ULONG STDMETHODCALLTYPE AddRef() override {
    return ++refCount;
  }

  ULONG STDMETHODCALLTYPE Release() override {
    return --refCount;
  }

  HRESULT STDMETHODCALLTYPE
  Invoke(HRESULT errorCode, ICoreWebView2Environment *result) override {
    return handleEnvCreated(errorCode, result);
  }

  HRESULT STDMETHODCALLTYPE
  Invoke(HRESULT errorCode, ICoreWebView2Controller *result) override {
    return handleCtrlCreated(errorCode, result);
  }

private:
  [[nodiscard]] HRESULT handleEnvCreated(
      HRESULT code, ICoreWebView2Environment *env);
  [[nodiscard]] HRESULT handleCtrlCreated(
      HRESULT code, ICoreWebView2Controller *ctrl);

  std::atomic_size_t refCount{0u};
  std::function<void(ICoreWebView2Controller *)> completeHandler{};
  HWND window{};

  FUSE_NONCOPYABLE(COMCreationHandler);
};

class COMEventHandler
    : public ICoreWebView2WebMessageReceivedEventHandler,
      public ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler {
public:
  static constexpr auto kWebMessageReceivedIID{
      __uuidof(ICoreWebView2WebMessageReceivedEventHandler)};
  static constexpr auto kInitScriptAddedIID{__uuidof(
      ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler)};

  constexpr COMEventHandler() noexcept = default;

  HRESULT STDMETHODCALLTYPE
  QueryInterface(REFIID riid, void **ppvObject) override {
    if (ppvObject == nullptr)
      return E_INVALIDARG;

    if (::IsEqualIID(riid, kWebMessageReceivedIID) ||
        ::IsEqualIID(riid, kInitScriptAddedIID)) {
      *ppvObject = this;
      return S_OK;
    } else {
      return E_NOINTERFACE;
    }
  }

  ULONG STDMETHODCALLTYPE AddRef() override {
    return ++refCount;
  }

  ULONG STDMETHODCALLTYPE Release() override {
    if (refCount <= 1u) {
      delete this;
      return 0u;
    } else {
      return --refCount;
    }
  }

  HRESULT STDMETHODCALLTYPE Invoke(
      ICoreWebView2 *core,
      ICoreWebView2WebMessageReceivedEventArgs *args) override {
    return handleWebMessageReceived(core, args);
  }

  HRESULT STDMETHODCALLTYPE Invoke(HRESULT errorCode, LPCWSTR result) override {
    return handleInitScriptAdded(errorCode, result);
  }

  fuse::Signal<void(const std::wstring &)> webMessageReceivedEvent{};
  fuse::Signal<void(bool, const std::wstring &)> initScriptAddedEvent{};

private:
  [[nodiscard]] HRESULT handleWebMessageReceived(
      ICoreWebView2 *core, ICoreWebView2WebMessageReceivedEventArgs *args);
  [[nodiscard]] HRESULT handleInitScriptAdded(HRESULT code, LPCWSTR result);

  std::atomic_size_t refCount{0u};

  FUSE_NONCOPYABLE(COMEventHandler);
};
}  // namespace hyperui::detail