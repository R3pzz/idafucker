#include <idafucker/resources/Resource.hpp>
#include <idafucker/resources/ResourceManager.hpp>

IDAFUCKER_NAMESPACE_BEGIN

bool Resource::load(const std::istream& stream)
{
  if (!empty()) {
    spdlog::debug(
        "Resource::load: called on a non-empty resource at {:X} of type '{}'",
        reinterpret_cast<std::uintptr_t>(this), data_.type().name());

    return true;
  }

  // Load dependencies if needed
  if (resolver_ != nullptr) {
    const auto succeeded =
        resolver_->resolve(stream)
            .and_then([this](const auto& dep_path_list) {
              for (auto&& dep_path : dep_path_list) {
                spdlog::debug("Resource::load: loading dependency {}",
                              dep_path.string());
                deps_.push_back(manager_.load(dep_path).raw());
              }
              return ResourceErrorOr<DependencyList>{dep_path_list};
            })
            .has_value();

    if (!succeeded) {
      spdlog::error(
          "Resource::load: could not resolve dependencies of a "
          "resource at {:X} of type '{}'",
          reinterpret_cast<std::uintptr_t>(this), data_.type().name());

      return false;
    }
  }

  data_ = factory_->construct(stream);
  if (data_.empty()) {
    spdlog::error(
        "Resource::load: could not construct a resource at {:X} of type '{}'",
        reinterpret_cast<std::uintptr_t>(this), data_.type().name());

    return false;
  }

  return true;
}

void Resource::unload()
{
  data_.reset();
  for (auto&& dep : deps_) {
    dep->onReferenceRemoved();
  }
  deps_.clear();
}

IDAFUCKER_NAMESPACE_END