#pragma once
#include <hyperui/CoreDefines.hpp>

#if not defined(interface)
  #include <combaseapi.h> // interface
#endif  // not defined(interface)

#include <WebView2.h>

// Callback, ComPtr, ...
#include <wrl.h>

HYPERUI_NAMESPACE_BEGIN

namespace impl::win
{

using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Callback;

} // namespace impl::win

HYPERUI_NAMESPACE_END