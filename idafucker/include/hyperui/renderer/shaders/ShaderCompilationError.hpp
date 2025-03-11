#pragma once
#include <hyperui/CoreDefines.hpp>

#include <GL/glew.h>

HYPERUI_NAMESPACE_BEGIN

class ShaderCompilationError {
public:
  constexpr ShaderCompilationError(GLuint shaderId) noexcept : _shaderId{shaderId} {}

  [[nodiscard]] std::string getMessage() const;

private:
  GLuint _shaderId{};
};

HYPERUI_NAMESPACE_END