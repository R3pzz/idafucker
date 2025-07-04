#pragma once
#include <fstream>        // ifstream
#include <unordered_map>  // unordered_map

#include <introspect/Config.hpp>
#include <introspect/resources/Resource.hpp>
#include <introspect/resources/ResourceFactory.hpp>

#include <spdlog/spdlog.h>

//
// A wrapper around an intrusive ref-counting cache.
// ResourceManager is used to centralize the resource
// management(caching/uncaching) and gather all resources into a single
// 'library' for easier access.
//
// Resource dependencies are not supported for now, this concept has
// to be reworked.
//

namespace introspect
{

// Stores resource registrations.
class ResourceManager {
public:
  constexpr ResourceManager() noexcept = default;

  // Load a resource from a file on disk.
  auto load(const std::filesystem::path& path, Resource::Flags flags)
      -> fuse::RCHandle<Resource>;

  // Construct a resource of a type `T` in-place. `associatedPath` is an
  // abstract path, by which the resource will be identified. In-place
  // resources can not be reloaded.
  template <class T>
  auto construct(const std::filesystem::path& associatedPath,
                 Resource::Flags flags,
                 auto&&... args) -> fuse::RCHandle<Resource>;

  // Refreshes the resource contents from its associated file.
  void reload(const std::filesystem::path& path);
  void reload(const fuse::RCHandle<Resource>& resource);

  // Unregistering resources.
  void erase(const std::filesystem::path& path, bool force = false);
  void erase(const fuse::RCHandle<Resource>& resource, bool force = false);

  // Check what resources should be uncached.
  void observe();

  // Retrieving a resource.
  [[nodiscard]] auto get(const std::filesystem::path& path) const
      -> fuse::RCHandle<Resource>;

  // Registering a resource factory.
  void registerFactory(const std::string& extension, ResourceFactory::Ref factory);

private:
  template <class T>
  using ExtensionMap = std::unordered_map<std::filesystem::path, std::shared_ptr<T>>;

  ExtensionMap<Resource> cache_{};             //< Resource cache.
  ExtensionMap<ResourceFactory> factories_{};  //< Resource factories.

  FUSE_NONCOPYABLE(ResourceManager);
};

template <class T>
auto ResourceManager::construct(const std::filesystem::path& associatedPath,
                                Resource::Flags flags,
                                auto&&... args) -> fuse::RCHandle<Resource> {
  const auto cache = cache_.find(associatedPath);
  if (cache != std::end(cache_))
    return fuse::RCHandle<Resource>{cache->second};

  auto resource = new Resource{std::in_place_type<T>, *this, flags,
                               std::forward<decltype(args)>(args)...};

  return fuse::RCHandle<Resource>{cache_.emplace(associatedPath, resource).first->second};
}

}  // namespace introspect