#pragma once
#include <memory>  // unique_ptr

#include <fuse/Class.hpp>
#include <introspect/Config.hpp>
#include <introspect/modules/Manifest.hpp>
#include <introspect/modules/ModuleId.hpp>
#include <introspect/resources/ResourceFactory.hpp>

#include <boost/uuid/uuid.hpp>
#include <nlohmann/json.hpp>

namespace introspect
{

class Module final {
public:
  // Constructs an empty, default module.
  constexpr Module() noexcept = default;

  [[nodiscard]] constexpr const ModuleId &id() const noexcept {
    return id_;
  }

  [[nodiscard]] constexpr const std::filesystem::path &path() const noexcept {
    return path_;
  }

  [[nodiscard]] static Module parse(const nlohmann::json &json);

private:
  // Used for path-independent module identification.
  ModuleId id_{};
  // Path to a folder containing the associated module.
  std::filesystem::path path_{};

  // Module manifest containing the metadata parsed from a `manifest.json` file.
  std::unique_ptr<Manifest> manifest_{};

  // The sandbox this module is being ran in.
  std::unique_ptr<Sandbox> sandbox_{};

  FUSE_NONCOPYABLE(Module);
};

class ModuleFactory final : public ResourceFactory {
public:
  enum class VerificationResult {
    Verified,
    RootNotFound,
    ManifestNotFound,
    NameMissing,
    VersionMissing,
    HashMismatch,
  };

  constexpr ModuleFactory(bool checkIntegrity) noexcept
      : checkIntegrity_{std::move(checkIntegrity)} {}

  [[nodiscard]] fuse::Any construct(const std::filesystem::path &path) const override;
  [[nodiscard]] const std::type_info &type() const noexcept override;
  
  [[nodiscard]] VerificationResult verify(
      const std::filesystem::path &path) const noexcept;

private:
  bool checkIntegrity_;
};

}  // namespace introspect