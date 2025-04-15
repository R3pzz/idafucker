#pragma once
#include <functional>     // function
#include <unordered_map>  // unordered_map

#include <hyperui/CoreDefines.hpp>

#include "IncludeWebView2.hpp"

#include <nlohmann/json.hpp>

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{
class InteropHostImpl final
    : public ICoreWebView2WebMessageReceivedEventHandler {
 public:
  using Binding = std::function<std::wstring(nlohmann::json)>;

  constexpr InteropHostImpl() noexcept = default;

  // Web-to-app comunication
  void bind(const std::string &eventName, const Binding &binding) noexcept
  {
    bindings_.insert_or_assign(eventName, binding);
  }

  void unbind(const std::string &eventName) noexcept
  {
    bindings_.erase(eventName);
  }

  // App-to-web communication
  
  
  // ICoreWebView2WebMessageReceivedEventHandler impl
  HRESULT STDMETHODCALLTYPE Invoke(
      ICoreWebView2 *sender,
      ICoreWebView2WebMessageReceivedEventArgs *args) override;

  // IUnknown impl
  HRESULT STDMETHODCALLTYPE
  QueryInterface(REFIID riid, void **ppvObject) override;

  ULONG STDMETHODCALLTYPE AddRef() override;
  ULONG STDMETHODCALLTYPE Release() override;

 private:
  ULONG refcount_{};
  std::unordered_map<std::string, Binding> bindings_{};
};
}  // namespace impl::win

HYPERUI_NAMESPACE_END