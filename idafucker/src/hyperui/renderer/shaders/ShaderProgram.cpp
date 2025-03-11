#include <hyperui/renderer/shaders/ShaderCompiler.hpp>
#include <hyperui/renderer/shaders/ShaderProgram.hpp>

#include <GL/glew.h>
#include <spdlog/spdlog.h>

HYPERUI_NAMESPACE_BEGIN

ShaderProgram::~ShaderProgram()
{
  // The resource should be brought out of memory when destructed
  if (_flags.contains(Flags::Cached))
    uncache();

  spdlog::debug("ShaderProgram::~ShaderProgram: called");
}

bool ShaderProgram::cache()
{
  if (_flags.contains(Flags::Cached)) {
    spdlog::debug(
        "ShaderResource::cache() called on a resource at '{:X}' that already satisfies "
        "'_flags.contains(Flags::Cached)'. "
        "Returning 'false'",
        reinterpret_cast<std::uintptr_t>(this));
    return false;
  }

  // Compile the vertex shader
  const auto vertId = ShaderCompiler::compileShaderFromBuffer(_vertexCode, GL_VERTEX_SHADER);
  if (!vertId) {
    spdlog::debug(
        "ShaderResource::cache() failed to compile shader with id '{}' for resource at '{:X}'. OpenGL error log: '{}'",
        *vertId, reinterpret_cast<std::uintptr_t>(this), vertId.error().getMessage());
    return false;
  }

  // Compile the fragment shader
  const auto fragId = ShaderCompiler::compileShaderFromBuffer(_fragmentCode, GL_FRAGMENT_SHADER);
  if (!fragId) {
    spdlog::debug(
        "ShaderResource::cache() failed to compile shader with id '{}' for resource at '{:X}'. OpenGL error log: '{}'",
        *fragId, reinterpret_cast<std::uintptr_t>(this), fragId.error().getMessage());
    return false;
  }

  // Merge them into a shader program
  _programId = glCreateProgram();
  glAttachShader(_programId, *vertId);
  glAttachShader(_programId, *fragId);
  glLinkProgram(_programId);

  glDeleteShader(*vertId);
  glDeleteShader(*fragId);

  // Get uniform locations
  _projMatrixLoc = glGetUniformLocation(_programId, "projMatrix");

  // Get attribute locations
  _positionLoc = glGetAttribLocation(_programId, "position");
  _colorLoc = glGetAttribLocation(_programId, "color");
  _uvLoc = glGetAttribLocation(_programId, "uv");

  _flags.add(Flags::Cached);

  return true;
}

bool ShaderProgram::uncache() { return true; }

HYPERUI_NAMESPACE_END