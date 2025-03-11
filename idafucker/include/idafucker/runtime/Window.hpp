#pragma once
#include "OpenGLContext.hpp"
#include "OpenGLContextSpecs.hpp"
#include "WindowVisibility.hpp"
#include <idafucker/CoreDefines.hpp>

#include <memory> // weak_ptr

IDAFUCKER_NAMESPACE_BEGIN

class Window {
public:
  using Handle = void *;

  virtual ~Window() = default;

  // Handle retrieval
  [[nodiscard]] virtual Handle rawHandle() const noexcept = 0;

  template <typename OSHandle>
  [[nodiscard]] auto handle() const noexcept
  {
    return reinterpret_cast<OSHandle>(rawHandle());
  }

  // Window behaviour
  [[nodiscard]] virtual WindowVisibility visibility() const = 0;
  virtual void changeVisibility(WindowVisibility value) = 0;

  [[nodiscard]] virtual std::wstring title() const = 0;
  virtual void changeTitle(const std::wstring &title) = 0;

  // OpenGL support
  [[nodiscard]] virtual std::weak_ptr<OpenGLContext> createOpenGLContext(const OpenGLContextSpecs &specs) = 0;
};

IDAFUCKER_NAMESPACE_END