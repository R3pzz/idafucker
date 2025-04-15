#include <idafucker/resources/ResourceManager.hpp>

IDAFUCKER_NAMESPACE_BEGIN

void ResourceManager::observe()
{
  for (auto&& resource : cache_) {
    if (!resource.second->hasReferences() && !resource.second->required()) {
      resource.second->unload();
    }
  }
}

auto ResourceManager::load(const std::filesystem::path& path)
    -> IntrusiveHandle<Resource>
{
  const auto cache = cache_.find(path.string());
  if (cache != cache_.end()) {
    spdlog::debug(
        "ResourceSystem::load: Resource '{}' of type '{}' is already "
        "present in the system",
        path.string(), cache->second->type().name());

    return IntrusiveHandle<Resource>{cache->second};
  }

  const auto ext = path.extension().string();

  // Look up the factory from the registration table
  const auto factory = factories_.find(ext);
  if (factory == factories_.end()) {
    spdlog::error(
        "ResourceSystem::load: No factory found for a resource '{}'",
        path.string());

    return {};
  }

  // Emplace a new resource
  const auto& resource =
      cache_
          .emplace(
              path.string(),
              new Resource{
                  *this,
                  factory->second,
                  resolvers_.contains(ext) ? resolvers_.at(ext) : nullptr,
                  {}})
          .first->second;
  // Open up the stream
  std::ifstream stream{path};
  if (stream.is_open() && !resource->load(stream)) {
    spdlog::warn(
        "ResourceSystem::load: Unable to load a resource '{}' of type '{}'",
        path.string(), factory->second->type().name());
  }

  return IntrusiveHandle<Resource>{resource};
}

void ResourceManager::erase(const std::filesystem::path& path, bool ignore_refs)
{
  const auto it = cache_.find(path.string());
  if (it == cache_.end()) {
    return;
  }

  auto resource = it->second;

  // In theory, we shouldn't encounter such situations
  if (resource == nullptr) [[unlikely]] {
    spdlog::debug(
        "ResourceManager::erase: found a null-pointer resource '{}'",
        path.string());

    cache_.erase(it);
    return;
  }

  // Do not unregister required resources
  if (resource->required() || (!ignore_refs && resource->hasReferences())) {
    return;
  }

  resource->unload();
  cache_.erase(it);
}

[[nodiscard]] auto ResourceManager::get(const std::filesystem::path& path) const
    -> IntrusiveHandle<Resource>
{
  const auto it = cache_.find(path.string());
  if (it == cache_.end()) {
    return {};
  }

  return IntrusiveHandle<Resource>{it->second};
}

IDAFUCKER_NAMESPACE_END