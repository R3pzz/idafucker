#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

enum class PixelFormat {
  RGBA, //< Red, green, blue and alpha channels
};

struct OpenGLContextSpecs {
  constexpr OpenGLContextSpecs() noexcept = default;

  PixelFormat format{};
  int colorDepth{};
  int depthBufferSize{};
  int stencilBufferSize{};
};

IDAFUCKER_NAMESPACE_END