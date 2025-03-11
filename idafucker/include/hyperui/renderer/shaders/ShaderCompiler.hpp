#pragma once
#include <hyperui/CoreDefines.hpp>
#include "ShaderCompilationError.hpp"

#include <GL/glew.h>

#include <expected> // expected
#include <filesystem> // path

HYPERUI_NAMESPACE_BEGIN

class ShaderCompiler {
public:
  template <typename Result>
  using ErrorOr = std::expected<Result, ShaderCompilationError>;
  
  // Creates an OpenGL shader and compiles it using code directly from a buffer
  [[nodiscard]] static ErrorOr<GLuint> compileShaderFromFile(const std::filesystem::path &path, GLenum type);
  // Creates an OpenGL shader and compiles it using code parsed from a file
  [[nodiscard]] static ErrorOr<GLuint> compileShaderFromBuffer(const std::string &buffer, GLenum type);

private:
  ShaderCompiler() = delete;
};

HYPERUI_NAMESPACE_END