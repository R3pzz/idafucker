#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/Window.hpp>
#include <idafucker/runtime/WindowSpecs.hpp>
#include "../../gl/windows/WindowsOpenGLContext.hpp"

#include <spdlog/spdlog.h>

IDAFUCKER_NAMESPACE_BEGIN

class WindowsWindow : public Window {
public:
  WindowsWindow(const WindowSpecs &specs, ATOM windowClassAtom);
  virtual ~WindowsWindow() override;

  [[nodiscard]] virtual Handle rawHandle() const noexcept override { return {_handle}; }

  [[nodiscard]] virtual WindowVisibility visibility() const override { return _visibility; }
  virtual void changeVisibility(WindowVisibility value) override;

  [[nodiscard]] virtual std::wstring title() const override;
  virtual void changeTitle(const std::wstring &title) override;

  [[nodiscard]] virtual std::weak_ptr<OpenGLContext> createOpenGLContext(
      const OpenGLContextSpecs &specs) override;

private:
  HWND _handle{};
  WindowVisibility _visibility{WindowVisibility::Hidden};
  std::shared_ptr<WindowsOpenGLContext> _glContext{};

  IDAFUCKER_NONCOPYABLE(WindowsWindow);
};

IDAFUCKER_NAMESPACE_END