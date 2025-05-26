#pragma once
#include <filesystem>  // path
#include <memory>      // shared_ptr
#include <vector>      // vector

#include <idafucker/Config.hpp>

//
// Abstract dependency resolver class that is implemented differently
// for different resource types.
//
// For example:
//
//  * HTML -> Parse HTML and extract all resources(images, style sheets,
//    scripts, etc...) that are referenced in it;
//  * Manifests -> Parse the .json file.
//

namespace idafucker
{
using DependencyList = std::vector<std::filesystem::path>;

class ResourceDependencyResolver {
public:
  using Ref = std::shared_ptr<ResourceDependencyResolver>;

  virtual ~ResourceDependencyResolver() noexcept = default;

  // Parse the resource and extract all dependencies
  [[nodiscard]] virtual DependencyList resolve(
      const std::istream& stream) const = 0;
};
}  // namespace idafucker