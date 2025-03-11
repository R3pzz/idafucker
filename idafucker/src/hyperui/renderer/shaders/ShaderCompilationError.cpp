#include <hyperui/renderer/shaders/ShaderCompilationError.hpp>

HYPERUI_NAMESPACE_BEGIN

[[nodiscard]] std::string ShaderCompilationError::getMessage() const {
  std::string message{};

  GLsizei length{};
  glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &length);
  message.resize(length);

  glGetShaderInfoLog(_shaderId, length, &length, message.data());
  return message;
}

HYPERUI_NAMESPACE_END