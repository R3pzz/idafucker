#pragma once
#include <hyperui/CoreDefines.hpp>
#include <idafucker/base/Flags.hpp>
#include <idafucker/runtime2/WindowOptions.hpp>

HYPERUI_NAMESPACE_BEGIN

struct HyperuiWindowOptions : idafucker::WindowOptions {
  enum class Hotkeys : std::uint32_t {
    None = 0u,
    F5 = 1u << 0u,
  };

  idafucker::Flags<Hotkeys> hotkeysToDisable{};
};

HYPERUI_NAMESPACE_END