#pragma once
#include <hyperui/CoreDefines.hpp>
#include <idafucker/base/math/Vector4.hpp>
#include "base/Color.hpp"
#include "shaders/ShaderProgram.hpp"

//
// A context for top-level OpenGL operations. For ex:
// 
//  RenderContext::clearBuffers() -> glClear()
//  RenderContext::setClearColor() -> glClearColor()
// 
// This also supports binding shaders, textures and etc...
// 
//  RenderContext::bindShader() -> glUseProgram()
//

HYPERUI_NAMESPACE_BEGIN

enum class BufferType : std::uint32_t {
  Color = GL_COLOR_BUFFER_BIT,
  Depth = GL_DEPTH_BUFFER_BIT,
};

class RenderContext final {
public:
  constexpr RenderContext() noexcept = default;

  void finish();

  void clearBuffers(idafucker::BitFlags<BufferType> type = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  void setClearColor(const Color &color);
  void setClearDepth(float value);

  void bindShader(std::shared_ptr<ShaderProgram> shader);

private:
  std::shared_ptr<ShaderProgram> _currentShader{};
};

HYPERUI_NAMESPACE_END