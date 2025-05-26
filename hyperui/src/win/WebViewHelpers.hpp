#pragma once
#define FUSE_EXPOSE_SYSTEM_HEADERS
#include <hyperui/Config.hpp>

#if not defined(interface)
  #include <combaseapi.h>  // interface
#endif

#include <WebView2.h>
#include <wrl.h> // Callback, ComPtr

namespace hyperui::detail
{

namespace wrl = Microsoft::WRL;

}  // namespace hyperui::detail