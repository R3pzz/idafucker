#include <idafucker/resources/ResourceManager.hpp>
#include <idafucker/exceptions/Exception.hpp>

IDAFUCKER_NAMESPACE_BEGIN

auto ResourceManager::load(
    const std::filesystem::path& path, const ResourceFlags flags)
    -> RcHandle<Resource>
{
  // Look for the resource in the cache
  const auto cache = cache_.find(path);
  if (cache != std::end(cache_))
    return RcHandle<Resource>{cache->second};

  // If not found, proceed to constructing it
  const auto extension = path.extension().string();
  const auto factory = factories_.find(extension);

  // Can't construct a resource without a factory...
  if (factory == std::end(factories_))
    throw Exception{"No factory registered for extension `"} << extension
                                                             << "`";

  // Now we can finally load the resource
  auto resource = new Resource{*this, flags, factory->second->construct(path)};
  return RcHandle<Resource>{cache_.emplace(path, resource).first->second};
}

void ResourceManager::reload(const std::filesystem::path& path)
{
  const auto it = cache_.find(path);
  if (it == std::end(cache_))
    return;

  const auto factory = factories_.at(it->first.extension().string());
  it->second->reset(factory->construct(it->first));
}

void ResourceManager::reload(const RcHandle<Resource>& resource)
{
  const auto it = std::find_if(
      std::begin(cache_), std::end(cache_),
      [&](const auto& pair) { return pair.second == resource; });
  if (it == std::end(cache_))
    return;

  const auto factory = factories_.at(it->first.extension().string());
  it->second->reset(factory->construct(it->first));
}

void ResourceManager::erase(const std::filesystem::path& path, bool force)
{
  const auto it = cache_.find(path);
  if (it == std::end(cache_))
    return;

  // Do not unregister required resources
  const auto &resource = it->second;
  if (!resource->required() && (force || resource->refCount() == 0u))
    cache_.erase(it); //< The resource will de-allocate itself on release
}

void ResourceManager::observe()
{
  for (auto it = std::begin(cache_); it != std::end(cache_); ++it) {
    if (it->second->refCount() == 0u && !it->second->required())
      cache_.erase(it);  //< The resource will de-allocate itself on release
  }
}

[[nodiscard]] auto ResourceManager::get(const std::filesystem::path& path) const
    -> RcHandle<Resource>
{
  const auto it = cache_.find(path);
  if (it == std::end(cache_))
    throw Exception{"Resource `"} << path.string() << "` not found";
  
  return RcHandle<Resource>{it->second};
}

void ResourceManager::registerFactory(
    const std::string& extension, ResourceFactory::Ref factory)
{
  factories_[extension] = std::move(factory);
}

IDAFUCKER_NAMESPACE_END