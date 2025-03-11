#include "WindowsWindowFactoryImpl.hpp"

IDAFUCKER_NAMESPACE_BEGIN

WindowsWindowFactoryImpl::WindowsWindowFactoryImpl()
{
  const auto instance = ::GetModuleHandleA(nullptr);
  IDAFUCKER_CHECK_NONZERO(instance);

  WNDCLASS windowClass{};
  windowClass.lpszClassName = L"IDAFUCKER_WINDOW_CLASS";
  windowClass.hInstance = instance;
  windowClass.lpfnWndProc = &::DefWindowProc;
  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  
  _windowClassAtom = ::RegisterClass(&windowClass);
  IDAFUCKER_CHECK_NONZERO(_windowClassAtom);
}

WindowsWindowFactoryImpl::~WindowsWindowFactoryImpl()
{
  const auto instance = ::GetModuleHandleA(nullptr);
  IDAFUCKER_CHECK_NONZERO(instance);

  ::UnregisterClass(MAKEINTATOM(_windowClassAtom), instance);
  _windowClassAtom = NULL;
}

IDAFUCKER_NAMESPACE_END