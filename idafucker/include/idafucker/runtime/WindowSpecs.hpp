#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/math/Common.hpp>

#include "WindowType.hpp"

IDAFUCKER_NAMESPACE_BEGIN

struct WindowSpecs {
  std::string title{};  //< Window title that is written in a native title bar
  Size size{};          //< Window size
  WindowType type{WindowType::Default};
  bool enableDpiSupport{true};

#if defined(IDAFUCKER_PLATFORM_WIN32)
  ATOM atom{};
#endif // defined(IDAFUCKER_PLATFORM_WIN32)

  [[nodiscard]] constexpr bool hasTitleBar() const noexcept
  {
    return title.empty();
  }
};

IDAFUCKER_NAMESPACE_END