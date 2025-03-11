#include <hyperui/renderer/RenderContext.hpp>

HYPERUI_NAMESPACE_BEGIN

void RenderContext::finish()
{
  glFlush();
}

void RenderContext::clearBuffers(idafucker::BitFlags<BufferType> type)
{
  glClear(type.get());
}

void RenderContext::setClearColor(const Color& color)
{
  glClearColor(color.r(), color.g(), color.b(), color.a());
}

void RenderContext::setClearDepth(float value)
{
  glClearDepth(value);
}

void RenderContext::bindShader(std::shared_ptr<ShaderProgram> shader)
{
  if (!shader->isCached())
    return;
  
  const auto id = shader->programId();
  glUseProgram(id);

  _currentShader = shader;
}

HYPERUI_NAMESPACE_END