#include <idafucker/resources/ResourceManager.hpp>

IDAFUCKER_NAMESPACE_BEGIN

void ResourceManager::observe()
{
  for (auto it = _cache.begin(); it != _cache.end();) {
    auto resource = it->second;

    if (resource->shouldUncache()) {
      resource->uncache();
      it = _cache.erase(it);
    } else {
      ++it;
    }
  }
}

ResourceManager resourceManager{};

IDAFUCKER_NAMESPACE_END