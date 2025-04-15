#include <hyperui/runtime/impl/win/InteropHostImpl.hpp>
#include <idafucker/base/String.hpp>

#include <spdlog/spdlog.h>

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{

HRESULT STDMETHODCALLTYPE InteropHostImpl::Invoke(
    ICoreWebView2 *sender, ICoreWebView2WebMessageReceivedEventArgs *args)
{
  LPWSTR message{};
  args->TryGetWebMessageAsString(&message);

  spdlog::debug(
      "InteropHostImpl::Invoke: message received: `{}`",
      idafucker::narrow(message));

  // Process the message
  auto jsonMessage = nlohmann::json::parse(idafucker::narrow(message));

  // Get the request ID
  const auto &request = jsonMessage["request"];
  if (request.empty())
    return E_INVALIDARG;

  const auto it = bindings_.find(request.get<std::string>());
  if (it == std::end(bindings_))
    return E_INVALIDARG;

  // Call the binding if it exists
  auto result = it->second(std::move(jsonMessage));
  sender->PostWebMessageAsJson(result.data());

  return S_OK;
}

HRESULT STDMETHODCALLTYPE
InteropHostImpl::QueryInterface(REFIID riid, void **ppvObject)
{
  constexpr IID HandlerGuid{
      0x57213f19,
      0x00e6,
      0x49fa,
      {0x8e, 0x07, 0x89, 0x8e, 0xa0, 0x1e, 0xcb, 0xd2}};

  if (::IsEqualIID(riid, HandlerGuid)) {
    *ppvObject = this;
    return S_OK;
  }

  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE InteropHostImpl::AddRef()
{
  return ++refcount_;
}

ULONG STDMETHODCALLTYPE InteropHostImpl::Release()
{
  if (refcount_ > 1u)
    return --refcount_;
  delete this;
  return 0u;
}

}  // namespace impl::win

HYPERUI_NAMESPACE_END