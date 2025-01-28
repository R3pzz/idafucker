#pragma once
#include <idafucker/CoreDefines.hpp>

#include <string> // wstring

IDAFUCKER_NAMESPACE_BEGIN

class WindowsAppWindow {
public:
  struct CreateParams {
    DWORD style{};
    DWORD styleEx{};
    RECT rect{};
    std::wstring title{};
    bool customTitleBar{};

    [[nodiscard]] constexpr int width() const noexcept { return rect.right - rect.left; }
    [[nodiscard]] constexpr int height() const noexcept { return rect.bottom - rect.top; }
  };

  WindowsAppWindow(const CreateParams &params, LPCWSTR className);
  constexpr WindowsAppWindow(const WindowsAppWindow &) noexcept = default;

  ~WindowsAppWindow();

  void setVisibility(bool visible);

  [[nodiscard]] constexpr HWND hwnd() const noexcept { return _hwnd; }

  static LRESULT WINAPI wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
  HWND _hwnd{};
};

IDAFUCKER_NAMESPACE_END