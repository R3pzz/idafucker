#pragma once
#include <hyperui/CoreDefines.hpp>

HYPERUI_NAMESPACE_BEGIN

enum class EventType {
  // Window mouse events are translated into RegionXXX events and sent over to subscribed widgets
  RegionClick,
  RegionHold,
  RegionEnter,
  RegionExit,

  // Key press events are filtered and sent over directly to the widgets
  KeyPress,
  KeyRelease,

  // A render event is triggered inside the main application thread
  Render,
};

HYPERUI_NAMESPACE_END