#pragma once
#include <filesystem>  // path

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/Any.hpp>

IDAFUCKER_NAMESPACE_BEGIN

class ResourceFactory {
public:
  using Ref = std::shared_ptr<ResourceFactory>;

  virtual ~ResourceFactory() noexcept = default;

  [[nodiscard]] virtual Any construct(
      const std::filesystem::path& dataPath) const = 0;
  [[nodiscard]] virtual const std::type_info& type() const noexcept = 0;
};

IDAFUCKER_NAMESPACE_END