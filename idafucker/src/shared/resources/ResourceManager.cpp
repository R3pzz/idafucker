#include <idafucker/resources/ResourceManager.hpp>

namespace idafucker
{
auto ResourceManager::load(
    const std::filesystem::path& path, const Resource::Flags flags)
    -> fuse::RCHandle<Resource> {
  // Look for the resource in the cache
  const auto cache = cache_.find(path);
  if (cache != std::end(cache_))
    return fuse::RCHandle<Resource>{cache->second};

  // If not found, proceed to constructing it
  const auto extension = path.extension().string();
  const auto factory = factories_.find(extension);

  // Can't construct a resource without a factory...
  if (factory == std::end(factories_))
    throw std::runtime_error{"no factory"};

  // Now we can finally load the resource
  auto resource = new Resource{*this, flags, factory->second->construct(path)};
  return fuse::RCHandle<Resource>{cache_.emplace(path, resource).first->second};
}

void ResourceManager::reload(const std::filesystem::path& path) {
  const auto it = cache_.find(path);
  if (it == std::end(cache_))
    return;

  const auto factory = factories_.at(it->first.extension().string());
  it->second->reset(factory->construct(it->first));
}

void ResourceManager::reload(const fuse::RCHandle<Resource>& resource) {
  const auto it = std::find_if(
      std::begin(cache_), std::end(cache_),
      [&](const auto& pair) { return pair.second == resource; });
  if (it == std::end(cache_))
    return;

  const auto factory = factories_.at(it->first.extension().string());
  it->second->reset(factory->construct(it->first));
}

void ResourceManager::erase(const std::filesystem::path& path, bool force) {
  const auto it = cache_.find(path);
  if (it == std::end(cache_))
    return;

  // Do not unregister required resources
  const auto& resource = it->second;
  if (!resource->isRequired() && (force || resource->refCount() == 0u))
    cache_.erase(it);  //< The resource will de-allocate itself on release
}

void ResourceManager::observe() {
  for (auto it = std::begin(cache_); it != std::end(cache_);) {
    if (it->second->refCount() == 0u && !it->second->isRequired())
      it =
          cache_.erase(it);  //< The resource will de-allocate itself on release
    else
      ++it;
  }
}

[[nodiscard]] auto ResourceManager::get(const std::filesystem::path& path) const
    -> fuse::RCHandle<Resource> {
  const auto it = cache_.find(path);
  if (it == std::end(cache_))
    throw std::runtime_error{"bad resource path"};

  return fuse::RCHandle<Resource>{it->second};
}

void ResourceManager::registerFactory(
    const std::string& extension, ResourceFactory::Ref factory) {
  factories_[extension] = std::move(factory);
}
}  // namespace idafucker