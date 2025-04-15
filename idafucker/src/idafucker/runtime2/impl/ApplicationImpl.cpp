#include <combaseapi.h>

#include <idafucker/base/CommandLine.hpp>
#include <idafucker/exceptions/PlatformException.hpp>
#include <idafucker/runtime2/impl/win/ApplicationImpl.hpp>
#include <idafucker/runtime2/impl/win/WindowImpl.hpp>

#include <spdlog/spdlog.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace
{
constexpr auto PerMonitorDpiAware{2};

[[nodiscard]] HRESULT WINAPI SetProcessDpiAwareness(int level)
{
  using Type = HRESULT(WINAPI)(int);

  const auto library =
      ::LoadLibrary(UTF8_OR_UNICODE_VARIANT("shcore.dll", L"shcore.dll"));
  if (library == NULL)
    return E_FAIL;

  const auto function = reinterpret_cast<Type *>(
      ::GetProcAddress(library, "SetProcessDpiAwareness"));
  if (function == nullptr)
    return E_FAIL;

  return function(level);
}
}  // namespace

namespace impl::win
{
ApplicationImpl::ApplicationImpl(const CommandLine<Character> &commandLine)
{
  ::CoInitialize(NULL);

  auto className = commandLine.find(L"--class-name");
  registerClass(className.valueOr(L"IdafuckerMain"));

  auto dpiSensitive = commandLine.find(L"--dpi-sensitive");
  if (dpiSensitive.isSet())
    configureDpi();
}

ApplicationImpl::~ApplicationImpl()
{
  terminationEvent.emit();
  unregisterClass();
  ::CoUninitialize();
}

void ApplicationImpl::runEventLoop() noexcept
{
  while (!awaitingTermination_) {
    MSG message{};
    while (::PeekMessage(&message, nullptr, 0u, 0u, PM_REMOVE)) {
      ::TranslateMessage(&message);
      ::DispatchMessage(&message);
    }
  }

  // Termination sequence
  terminationEvent.emit();
}

void ApplicationImpl::registerClass(View name)
{
  WNDCLASSEX wc{
      .cbSize = sizeof(WNDCLASSEX),
      .style = CS_HREDRAW | CS_VREDRAW,
      .lpfnWndProc = &windowProc,
      .hInstance = ::GetModuleHandle(nullptr),
      .hIcon = NULL,
      .hCursor = NULL,
      .lpszClassName = name.data(),
  };

  if (wcAtom_ = ::RegisterClassEx(&wc); wcAtom_ == NULL)
    throw PlatformException{"Failed to register window class"};
}

void ApplicationImpl::unregisterClass() noexcept
{
  ::UnregisterClass(MAKEINTATOM(wcAtom_), ::GetModuleHandle(nullptr));
}

void ApplicationImpl::configureDpi()
{
  if (::SetProcessDpiAwarenessContext(
          DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE) != TRUE)
    throw PlatformException{"Failed to set process' DPI awareness context"};

  if (const auto result = SetProcessDpiAwareness(PerMonitorDpiAware);
      result != S_OK && result != E_ACCESSDENIED)
    throw PlatformException{"Failed to set process' DPI awareness"};

  if (::SetProcessDPIAware() != TRUE)
    throw PlatformException{"Failed to make the process DPI aware"};
}

[[nodiscard]] LRESULT CALLBACK ApplicationImpl::windowProc(
    HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  auto window =
      reinterpret_cast<WindowImpl *>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
  if (window == nullptr) {
    spdlog::debug(
        "ApplicationImpl::windowProc: An unregistered window `0x{:X}` "
        "encountered.",
        reinterpret_cast<std::uintptr_t>(hwnd));
    return ::DefWindowProc(hwnd, message, wparam, lparam);
  }

  // Let the window handle the message
  return window->onWindowMesasge(message, wparam, lparam);
}
}  // namespace impl::win

IDAFUCKER_NAMESPACE_END