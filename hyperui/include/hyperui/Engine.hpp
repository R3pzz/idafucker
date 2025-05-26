#pragma once
#include <fuse/Class.hpp>
#include <fuse/Signal.hpp>
#include <hyperui/Config.hpp>
#include <idafucker/math/Common.hpp>

#include "Markup.hpp"

#include <nlohmann/json.hpp>

namespace hyperui
{

class Engine {
public:
  constexpr Engine() noexcept = default;
  virtual ~Engine() noexcept = default;

  // Markup loaders.
  virtual void loadFromMarkup(const Markup &markup) = 0;
  virtual void loadFromURL(const std::wstring &url) = 0;

  // Native JavaScript API support.
  virtual void addJSInitializationScript(const std::wstring &script) = 0;
  virtual void postJSMessage(const std::wstring &message) noexcept = 0;

  // Window-specific callbacks
  virtual void handleResizeEvent(const idafucker::Rectangle<int> &bounds) = 0;

  // Events
  fuse::Signal<void(nlohmann::json)> jsMessageReceivedEvent{};

private:
  FUSE_NONCOPYABLE(Engine);
};

}  // namespace hyperui