#define FUSE_EXPOSE_SYSTEM_HEADERS
#include "COMHandlers.hpp"

namespace hyperui::detail
{

[[nodiscard]] bool COMCreationHandler::requestWebView2Creation(
    const std::wstring &udf) {
  return SUCCEEDED(::CreateCoreWebView2EnvironmentWithOptions(
      nullptr, udf.c_str(), nullptr, this));
}

HRESULT COMCreationHandler::handleEnvCreated(
    HRESULT code, ICoreWebView2Environment *env) {
  if (FAILED(code)) [[unlikely]]
    completeHandler(nullptr);
  else
    env->CreateCoreWebView2Controller(window, this);
  
  return S_OK;
}

HRESULT COMCreationHandler::handleCtrlCreated(
    HRESULT code, ICoreWebView2Controller *ctrl) {
  if (FAILED(code)) [[unlikely]]
    completeHandler(nullptr);
  else
    completeHandler(ctrl);
  return S_OK;
}

[[nodiscard]] HRESULT COMEventHandler::handleWebMessageReceived(
    ICoreWebView2 *core, ICoreWebView2WebMessageReceivedEventArgs *args) {
  LPWSTR message{};
  args->get_WebMessageAsJson(&message);
  webMessageReceivedEvent.emit(message);
  return S_OK;
}

[[nodiscard]] HRESULT COMEventHandler::handleInitScriptAdded(
    HRESULT code, LPCWSTR result) {
  if (FAILED(code))
    initScriptAddedEvent.emit(false, result);
  else
    initScriptAddedEvent.emit(true, result);
  return S_OK;
}

}  // namespace hyperui::detail