#pragma once
#include <fstream>        // ifstream
#include <unordered_map>  // unordered_map

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/RefCount.hpp>

#include "Resource.hpp"
#include "ResourceError.hpp"

#include <spdlog/spdlog.h>

//
// A wrapper around an intrusive ref-counting cache.
// ResourceManager is used to centralize the resource
// management(caching/uncaching) and gather all resources into a single
// 'library' for easier access.
//
// Resource caching:
//
//  * Resource::cache() -> loads the resource from its source file.
//  * Resource::uncache() -> frees the resource data.
//
// Resource lifetime:
//
//  * When a resource is added to the system, its type object is by default
//  unset;
//  * When cache() is called, resource data is loaded into a temporary buffer,
//  passed
//    to the dependency resoler(where the process repeats for its dependencies)
//    and the associated factory for object instantiation.
//  * When uncache() is called, the resource is freed from the memory and the
//  reference
//    count on all dependencies is decremented.
//  * When ResourceManager::releaseResource() is called, the resource is first
//  uncached
//    and then removed from the library.
//
// Resource type identification guidelines:
//
//  * Should be unified for all classes that rely on it;
//  * Should be easily deduced from an extension and vice-versa;
//  * Should be of a relatively small size;
//  * Should be comparable with std::type_info.
//

IDAFUCKER_NAMESPACE_BEGIN

// Stores resource registrations.
class ResourceManager {
 public:
  constexpr ResourceManager() noexcept = default;

  void pushFactory(const std::string& extension, ResourceFactory::Ref factory)
  {
    factories_[extension] = std::move(factory);
  }

  void pushFesolver(
      const std::string& extension, ResourceDependencyResolver::Ref resolver)
  {
    resolvers_[extension] = std::move(resolver);
  }

  // Load a resource and cache it immediately
  auto load(const std::filesystem::path& path) -> RcHandle<Resource>;

  // Construct a resource of a type 'T'
  // FIXME: Dependencies are currently not supported
  template <class T, typename... Args>
  auto construct(const std::string& name, Args&&... args)
      -> RcHandle<Resource>;

  // Unregistering resources
  void erase(const std::filesystem::path& path, bool ignore_refs = false);

  // Check what resources should be uncached
  void observe();

  // Retrieving a resource
  [[nodiscard]] auto get(const std::filesystem::path& path) const
      -> RcHandle<Resource>;

 private:
  template <class T> using ExtensionMap = std::unordered_map<std::string, T>;

  ExtensionMap<std::shared_ptr<Resource>> cache_{};  //< Resource cache
  ExtensionMap<std::shared_ptr<ResourceFactory>>
      factories_{};  //< Resource factories
  ExtensionMap<std::shared_ptr<ResourceDependencyResolver>>
      resolvers_{};  //< Resource dependency resolvers

  IDAFUCKER_NONCOPYABLE(ResourceManager);
};

template <class T, typename... Args>
auto ResourceManager::construct(const std::string& name, Args&&... args)
    -> RcHandle<Resource>
{
}

IDAFUCKER_NAMESPACE_END