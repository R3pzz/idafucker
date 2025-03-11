#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

enum class WindowVisibility {
  Shown, //< Activate and expose the window to the user
  Hidden, //< Hide the window from the user
  Minimized, //< Minimize the window to tray
  Maximized, //< Make the window fit the screen
};

IDAFUCKER_NAMESPACE_END