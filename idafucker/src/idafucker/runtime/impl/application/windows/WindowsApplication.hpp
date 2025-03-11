#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/Application.hpp>

#include <vector> // vector

IDAFUCKER_NAMESPACE_BEGIN

class WindowsApplication final : public Application {
public:
  WindowsApplication(int argc, char *argv[]);
  virtual ~WindowsApplication() override;

  [[nodiscard]] virtual bool running() const override { return _running; }

  virtual void requestTermination() override { _terminationRequested = true; }
  [[nodiscard]] virtual bool terminationRequested() const override { return _terminationRequested; }

  virtual void registerWindow(const std::shared_ptr<Window> &window) override { _windows.push_back(window); }
  virtual void releaseWindow(const std::shared_ptr<Window> &window) override
  {
    std::remove(_windows.begin(), _windows.end(), window);
  }

  [[nodiscard]] virtual std::shared_ptr<Window> primaryWindow() const override { return _windows[0u]; }
  [[nodiscard]] virtual bool isPrimaryWindow(const std::shared_ptr<Window> &window) const override
  {
    return _windows[0u] == window;
  }

private:
  bool _running{true};
  bool _terminationRequested{false};
  std::vector<std::shared_ptr<Window>> _windows{};

  IDAFUCKER_NONCOPYABLE(WindowsApplication);
};

IDAFUCKER_NAMESPACE_END