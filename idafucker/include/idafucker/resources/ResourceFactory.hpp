#pragma once
#include <filesystem>  // path

#include <idafucker/Config.hpp>

#include <fuse/Any.hpp>

namespace idafucker
{
class ResourceFactory {
public:
  using Ref = std::shared_ptr<ResourceFactory>;

  virtual ~ResourceFactory() noexcept = default;

  [[nodiscard]] virtual fuse::Any construct(
      const std::filesystem::path& dataPath) const = 0;
  [[nodiscard]] virtual const std::type_info& type() const noexcept = 0;
};
}  // namespace idafucker