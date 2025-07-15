#pragma once
#include <string>  // string_view
#include <vector>  // vector

#include <introspect/Config.hpp>

#include <fuse/Bits.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace introspect
{

class ModuleFeatures final {
public:
  enum Value : std::uint32_t {
    None = 0u,  //< The module does not require any sandbox priviliges to execute.
    LocalFilesystem = 1u
                      << 0u,  //< By "local filesystem" we mean that the host(introspect
                              // engine) will create a dedicated folder for this plugin's
                              // data, and the access to this folder will be given to the
                              // plugin, essentially creating a "local" mini-filesystem
                              // for it. Maximum reserved storage size for a single
                              // plugin is 4MB.
    // We do not allow plugins to access the global filesystem, as it is a potential
    // (and a very probable) source of security issues, for obvious reasons.
    ProcessTree = 1u << 1u,  //< We also tend to normally restrict access to the process
                             // tree and other processes, also for security reasons.
  };

  constexpr ModuleFeatures() noexcept = default;
  constexpr ModuleFeatures(Value value) noexcept : value_{std::move(value)} {}

  [[nodiscard]] constexpr bool operator==(const ModuleFeatures& rhs) const noexcept {
    return value_ == rhs.value_;
  }

  [[nodiscard]] constexpr bool operator!=(const ModuleFeatures& rhs) const noexcept {
    return value_ != rhs.value_;
  }

  [[nodiscard]] constexpr Value value() const noexcept {
    return static_cast<Value>(value_.get());
  }

  [[nodiscard]] constexpr bool has(const Value value) const noexcept {
    return value_.contains(value);
  }

  [[nodiscard]] constexpr bool hasLocalFilesystem() const noexcept {
    return value_.contains(LocalFilesystem);
  }

  [[nodiscard]] constexpr bool hasProcessTree() const noexcept {
    return value_.contains(ProcessTree);
  }

  [[nodiscard]] static ModuleFeatures parse(const nlohmann::json& list);

private:
  fuse::Bits<Value> value_{};
};

}  // namespace introspect