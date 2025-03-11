#pragma once
#include <hyperui/CoreDefines.hpp>
#include <idafucker/resources/Resource.hpp>

#include <GL/glew.h>

HYPERUI_NAMESPACE_BEGIN

class ShaderProgram : public idafucker::Resource {
public:
  using Ref = std::shared_ptr<ShaderProgram>;

  constexpr ShaderProgram() noexcept = default;
  constexpr ShaderProgram(
      const std::string &vertexCode, const std::string &fragmentCode, idafucker::Resource::Flags flags = {}) noexcept
      : idafucker::Resource{flags}, _vertexCode{vertexCode}, _fragmentCode{fragmentCode} {}

  ~ShaderProgram();
  
  IDAFUCKER_IMPLEMENT_RESOURCE_TYPE();

  // Allocates a shader from the source code file
  virtual bool cache();
  // Releases the shader
  virtual bool uncache();

  // Property getters
  [[nodiscard]] constexpr GLuint programId() const noexcept { return _programId; }

private:
  // The source code this thing gets compiled from
  std::string _vertexCode{}, _fragmentCode{};

  // The shader itself
  GLuint _programId{};

  // Shader uniform/attribute locations
  GLuint _projMatrixLoc{}, _positionLoc{}, _colorLoc{}, _uvLoc{};
};

HYPERUI_NAMESPACE_END