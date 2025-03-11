#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/runtime/WindowFactory.hpp>

IDAFUCKER_NAMESPACE_BEGIN

class WindowsWindowFactoryImpl : public WindowFactoryImpl {
public:
  WindowsWindowFactoryImpl();
  ~WindowsWindowFactoryImpl();

  [[nodiscard]] constexpr auto windowClassAtom() const noexcept { return _windowClassAtom; }

private:
  ATOM _windowClassAtom{};
};

IDAFUCKER_NAMESPACE_END