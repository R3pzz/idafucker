#include <hyperui/renderer/shaders/ShaderCompiler.hpp>

#include <GL/glew.h>

#include <fstream> // ifstream

HYPERUI_NAMESPACE_BEGIN

[[nodiscard]] ShaderCompiler::ErrorOr<GLuint> ShaderCompiler::compileShaderFromFile(
    const std::filesystem::path &path, GLenum type) {
  std::ifstream inputFile{path};
  if (!inputFile.is_open())
    return false;

  std::string buf{std::istreambuf_iterator<char>{inputFile}, std::istreambuf_iterator<char>{}};
  inputFile.close();

  return compileShaderFromBuffer(buf, type);
}

[[nodiscard]] ShaderCompiler::ErrorOr<GLuint> ShaderCompiler::compileShaderFromBuffer(
    const std::string &buffer, GLenum type) {
  const auto id = glCreateShader(type);

  const auto bufferPtr = buffer.c_str();
  glShaderSource(id, 1u, &bufferPtr, nullptr);
  glCompileShader(id);

  GLsizei result{};
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (!result)
    return std::unexpected(ShaderCompilationError{id});
  
  return id;
}

HYPERUI_NAMESPACE_END