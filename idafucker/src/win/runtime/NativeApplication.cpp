#define FUSE_EXPOSE_SYSTEM_HEADERS
#include "NativeApplication.hpp"
#include "NativeWindow.hpp"

#include <objbase.h>

#include <spdlog/spdlog.h>

namespace idafucker::detail
{
NativeApplication::NativeApplication(
    const fuse::CommandLine<wchar_t> &commandLine) {
  // Initialize COM
  ::CoInitialize(NULL);

  // Register the window class
  registerClass();

  // Configure DPI if requested
  if (commandLine.find(L"--dpi-sensitive").isSet())
    configureDpi();
}

NativeApplication::~NativeApplication() noexcept {
  unregisterClass();
  ::CoUninitialize();
}

void NativeApplication::runEventLoop(
    const std::function<void()> &loopFunc) noexcept {
  MSG message{};
  while (::PeekMessageW(&message, nullptr, 0u, 0u, PM_REMOVE)) {
    ::TranslateMessage(&message);
    ::DispatchMessageW(&message);
  }

  try {
    if (loopFunc != nullptr)
      loopFunc();
  } catch (std::exception &e) {
    spdlog::error("Unhandled exception in event loop: {}", e.what());
    return;
  }
}

void NativeApplication::registerClass() {
  WNDCLASSEX wc{
      .cbSize = sizeof(WNDCLASSEX),
      .style = CS_HREDRAW | CS_VREDRAW,
      .lpfnWndProc = &NativeWindow::nativeMessageHandler,
      .hInstance = ::GetModuleHandle(nullptr),
      .hIcon = NULL,
      .hCursor = NULL,
      .lpszClassName = kClassName,
  };

  if (winAtom_ = ::RegisterClassExW(&wc); winAtom_ == NULL)
    throw std::runtime_error{"failed registering window class"};
}

void NativeApplication::unregisterClass() noexcept {
  ::UnregisterClass(MAKEINTATOM(winAtom_), ::GetModuleHandle(nullptr));
}

void NativeApplication::configureDpi() {
  constexpr auto kPerMonitorDpiAware{2};

  const auto setProcessDpiAwareness = [](int awareness) -> HRESULT {
    using Fn = HRESULT(WINAPI)(int);

    const auto library = ::LoadLibraryW(L"shcore.dll");
    if (library == NULL)
      return E_FAIL;

    const auto function = reinterpret_cast<Fn *>(
        ::GetProcAddress(library, "SetProcessDpiAwareness"));
    if (function == nullptr)
      return E_FAIL;

    return function(awareness);
  };

  if (::SetProcessDpiAwarenessContext(
          DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE) != TRUE)
    throw std::runtime_error{"failed setting process' DPI awareness context"};

  if (const auto result = setProcessDpiAwareness(kPerMonitorDpiAware);
      result != S_OK && result != E_ACCESSDENIED)
    throw std::runtime_error{"failed setting process' DPI awareness"};

  if (::SetProcessDPIAware() != TRUE)
    throw std::runtime_error{"failed making the process DPI aware"};
}
}  // namespace idafucker::detail