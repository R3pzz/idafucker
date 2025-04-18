#pragma once
#include <hyperui/CoreDefines.hpp>

#if not defined(interface)
  #include <combaseapi.h>  // interface
#endif                     // not defined(interface)

#include <WebView2.h>

// Callback, ComPtr, ...
#include <wrl.h>

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{
using Microsoft::WRL::Callback;
using Microsoft::WRL::ComPtr;

template <class HandlerType>
[[nodiscard]] auto makeHander(auto &&callback) noexcept
{
  return Callback<HandlerType>(std::forward<decltype(callback)>(callback));
}

namespace handlers
{
using EnvCreated = ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler;
using CtrlCreated = ICoreWebView2CreateCoreWebView2ControllerCompletedHandler;
using MessageReceived = ICoreWebView2WebMessageReceivedEventHandler;
using InitScriptAdded = ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler;
}  // namespace handlers
}  // namespace impl::win

HYPERUI_NAMESPACE_END