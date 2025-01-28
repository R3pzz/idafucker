#pragma once
#include "WindowsWindow.hpp"

#include <vector> // vector

IDAFUCKER_NAMESPACE_BEGIN

class WindowsApp final {
public:
  WindowsApp(int argc, char *argv[]);
  ~WindowsApp();

  void setIcon(HICON icon);
  void setCursor(HCURSOR icon);

  [[nodiscard]] WindowsAppWindow &createWindow(const WindowsAppWindow::CreateParams &params);

  [[nodiscard]] constexpr auto &window(std::size_t idx) { return _windows.at(idx); }
  [[nodiscard]] constexpr auto running() const noexcept { return _running; }

  [[nodiscard]] static constexpr LPCWSTR windowClassName() noexcept { return L"idafucker_window"; }

private:
  std::vector<WindowsAppWindow> _windows{};
  bool _running{true};

  IDAFUCKER_NONCOPYABLE(WindowsApp);
  IDAFUCKER_NONMOVABLE(WindowsApp);
};

IDAFUCKER_NAMESPACE_END