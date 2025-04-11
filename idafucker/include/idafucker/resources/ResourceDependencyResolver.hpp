#pragma once
#include <idafucker/CoreDefines.hpp>

#include "ResourceError.hpp"

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

IDAFUCKER_NAMESPACE_BEGIN

using DependencyList = std::vector<std::filesystem::path>;

class ResourceDependencyResolver {
 public:
  using Ref = std::shared_ptr<ResourceDependencyResolver>;

  virtual ~ResourceDependencyResolver() noexcept = default;

  // Parse the resource and extract all dependencies
  [[nodiscard]] virtual auto resolve(const std::istream& stream) const
      -> ResourceErrorOr<DependencyList> = 0;
};

IDAFUCKER_NAMESPACE_END