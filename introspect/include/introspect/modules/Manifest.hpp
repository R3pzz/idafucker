#pragma once
#include <optional>    // optional
#include <filesystem>  // path
#include <string>      // string

#include <introspect/Config.hpp>
#include <introspect/modules/ModuleHash.hpp>
#include <introspect/modules/ModuleFeatures.hpp>
#include <introspect/modules/ModuleVersion.hpp>
#include <introspect/resources/ResourceFactory.hpp>
#include <fuse/Class.hpp>

#include <nlohmann/json.hpp>

namespace introspect
{

class Manifest final {
public:
  // Constructs an empty manifest with no data.
  constexpr Manifest() noexcept = default;
  Manifest(nlohmann::json json) noexcept : data_{std::move(json)} {}

  [[nodiscard]] constexpr bool empty() const noexcept {
    return data_.empty();
  }

  // Direct value getters
  [[nodiscard]] std::optional<std::string> name() const noexcept {
    return find<std::string>("name");
  }

  [[nodiscard]] std::optional<std::string> description() const noexcept {
    return find<std::string>("description");
  }

  [[nodiscard]] std::optional<ModuleVersion> version() const noexcept {
    return find<ModuleVersion>("version");
  }

  [[nodiscard]] std::optional<ModuleHash> hash() const noexcept {
    return find<ModuleHash>("hash");
  }

  [[nodiscard]] std::optional<ModuleFeatures> features() const noexcept {
    return find<ModuleFeatures>("features");
  }

private:
  template <typename Type>
  [[nodiscard]] auto find(const char *key) const -> std::optional<Type> {
    if (data_.contains(key)) {
      if constexpr (requires { Type::parse(data_["key"]); }) {
        return std::make_optional(Type::parse(data_["key"]));
      } else {
        return std::make_optional(data_["key"].get<Type>());
      }
    }

    return std::nullopt;
  }

  nlohmann::json data_{};

  FUSE_NONCOPYABLE(Manifest);
};

}  // namespace introspect