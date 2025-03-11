#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/math/Common.hpp>
#include "WindowType.hpp"

IDAFUCKER_NAMESPACE_BEGIN

struct WindowSpecs {
  constexpr WindowSpecs() noexcept = default;

  [[nodiscard]] constexpr bool hasTitleBar() const noexcept { return title.empty(); }

  std::wstring title{}; //< Window title that is written in a native title bar
  Extent extent{}; //< Window size in a vector [float, float] rounded up to [int, int] using static_cast rules
  WindowType type{WindowType::Default};
};

IDAFUCKER_NAMESPACE_END