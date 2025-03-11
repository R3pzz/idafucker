#pragma once
#include <idafucker/CoreDefines.hpp>
#include "WindowSpecs.hpp"
#include "Window.hpp"

#include <memory> // shared_ptr, unique_ptr

IDAFUCKER_NAMESPACE_BEGIN

class WindowFactoryImpl {
public:
  constexpr WindowFactoryImpl() noexcept = default;
};

class WindowFactory {
public:
  WindowFactory();

  [[nodiscard]] std::shared_ptr<Window> create(const WindowSpecs &specs) const;

private:
  std::unique_ptr<WindowFactoryImpl> _impl{};
};

extern WindowFactory windowFactory;

IDAFUCKER_NAMESPACE_END