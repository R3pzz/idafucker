#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/templates/Cache.hpp>
#include "Resource.hpp"
#include "ResourceRef.hpp"

#include <spdlog/spdlog.h>

#include <unordered_map> // unordered_map

//
// A wrapper around LRU cache.
// ResourceManager is used to centralize the resource management(caching/uncaching)
// and gather all resources into a single library for easier access.
//
// Resource caching:
//
//  * Resource::cache() -> loads the resource from its source file.
//  * Resource::uncache() -> frees the resource data.
//
// Resource lifetime:
//
//  * When a resource is created, it is automatically registered in the resource system
//    and its reference count is incremented.
//  * When the reference count comes to zero, the resource is force
//

IDAFUCKER_NAMESPACE_BEGIN

// Stores resource registrations.
class ResourceManager {
public:
  constexpr ResourceManager() noexcept = default;

  // Constructs and registers an empty resource of type 'T'
  template <typename T, typename... Args>
    requires std::is_base_of<Resource, T>::value
  auto createProceduralResource(const std::string &name, Args &&...args)
  {
    // Check if we already have such resource registered
    const auto it = _cache.find(name);
    if (it != _cache.end()) {
      // Might as well print a stack trace here
      spdlog::debug(
          "ResourceSystem::createProceduralResource: Resource '{}' of type '{}' is already present in the system.",
          name, typeid(T).name());

      return ResourceRef<T>{it->second};
    }

    // Allocate an empty resource and construct it
    auto &resource = _cache[name];
    resource = std::make_shared<T>(std::forward<Args>(args)...);

    return ResourceRef<T>{resource};
  }

  // Immediately loads a resource of type 'T' from disk
  template <typename T, typename... Args>
    requires std::is_base_of<Resource, T>::value
  auto immediateLoadResource(const std::string &name, Args &&...args)
  {
    // Check if we already have such resource registered
    const auto it = _cache.find(name);
    if (it != _cache.end()) {
      // Might as well print a stack trace here
      spdlog::debug(
          "ResourceSystem::immediateLoadResource: Resource '{}' of type '{}' is already present in the system.",
          name, typeid(T).name());

      return ResourceRef<T>{it->second};
    }

    // Allocate an empty resource, construct and cache it
    auto &resource = _cache[name];
    resource = std::make_shared<T>(std::forward<Args>(args)...);
    resource->cache();

    return ResourceRef<T>{resource};
  }

  // Unregistering resources
  template <typename T>
  void releaseResource(const std::string& name, bool ignoreReferences = false)
  {
    const auto it = _cache.find(name);
    if (it == _cache.end())
      return;

    auto resource = it->second;

    // In theory, we shouldn't encounter such situations
    if (resource == nullptr) [[unlikely]] {
      spdlog::debug("ResourceManager::releaseResource: found a null-pointer resource '{}'", name);

      _cache.erase(it);
      return;
    }

    // Do not unregister required resources
    if (resource->isRequired()) {
      spdlog::debug(
          "ResourceManager::releaseResource: request to release a required resource '{}' of type '{}' declined",
          name, resource->type().typeName());

      return;
    }

    // Only release if not referenced
    if (!ignoreReferences && resource->isReferenced())
      return;

    if (resource->type() != TypeFamily<T>::value())
      return;

    resource->uncache();
    _cache.erase(it);
  }

  // Retrieving a resource
  template <typename T>
  [[nodiscard]] std::shared_ptr<T> get(const std::string &name) const
  {
    const auto it = _cache.find(name);
    if (it == _cache.end())
      return ResourceRef<T>{};

    auto resource = it->second;

    if (resource->type() != TypeFamily<T>::value()) {
      // Might as well print a stack trace here
      spdlog::debug(
          "ResourceSystem::get: Requested resource's ('{}') type '{}' does not match the existing one's '{}'. "
          "Returning an empty handle",
          name, typeid(T).name(), resource->type().typeName());

      return ResourceRef<T>{};
    }

    return ResourceRef<T>{std::static_pointer_cast<T>(resource)};
  }

  // Presence checking
  template <typename T>
  [[nodiscard]] bool contains(const std::string &name) const
  {
    const auto it = _cache.find(name);
    if (it == _cache.end())
      return false;

    return it->second->type() == TypeFamily<T>::value();
  }

  // Check what resources should be unregistered
  void observe();

private:
  std::unordered_map<std::string, std::shared_ptr<Resource>> _cache{};

  IDAFUCKER_NONCOPYABLE(ResourceManager);
};

extern ResourceManager resourceManager;

IDAFUCKER_NAMESPACE_END