#pragma once
#include "BaseWidget.hpp"
#include <hyperui/CoreDefines.hpp>
#include <idafucker/resources/Resource.hpp>

#include <boost/signals2.hpp>

HYPERUI_NAMESPACE_BEGIN

class CheckboxWidget : public BaseWidget {
public:
  using Ref = std::shared_ptr<CheckboxWidget>;

  constexpr CheckboxWidget(const std::string &title) noexcept : _title{title} {}
  CheckboxWidget(const std::string &title, StyleRef style) noexcept : BaseWidget{style}, _title{title} {}

  // Draws the checkbox according to its style
  virtual void render() override;

  // Processes the events.
  // Accepted events:
  //  - RegionClick
  //  - RegionEnter
  //  - RegionExit
  //  - RegionHold
  //  - KeyPress
  //  - KeyRelease
  //  - Render
  virtual void onEvent(EventRef event) override;

  // Direct state getters/setters
  [[nodiscard]] auto enabled() const noexcept { return _enabled.load(); }

  void enable();
  void disable();
  void toggle();

  // Callbacks
  auto connectStateChanged(const boost::signals2::slot<void(bool)> &slot) { return _stateChanged.connect(slot); }

private:
  // Keep track of our state
  std::atomic<bool> _enabled{};

  // The title
  std::string _title{};

  // Signal propagation/event handling
  boost::signals2::signal<void(bool)> _stateChanged{};
};

HYPERUI_NAMESPACE_END

// Threading architecture:
// LAYER 1
//  - Threads
//    - Main loop thread/bootstrapper
//    - Window/application event processing
//  - Jobs
//    - Resource caching using asynchronous file loading
//    - Dynamic module loading
// LAYER 2
//  - Threads
//    - Event distribution thread
//      Receives events from the application event processor,
//      distributes the events to all subscribed widgets.
//      For ex:
//      It is time to render the UI. Event distributor sends a RenderEvent
//      to each subscribed widget, and each of the widgets that receive
//      that event render themselves using the rendering thread, which runs
//      asynchronously and processes the draw requests.
//    - Rendering thread
//      Receives draw requests from the widgets, processes them, translates
//      them into batches and renders using draw calls to OpenGL.
//  - Jobs
//    - Asynchronous style loading using tinyXML
// LAYER 3
//