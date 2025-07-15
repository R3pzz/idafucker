#include <fstream> // ifstream

#include <introspect/modules/Module.hpp>

namespace introspect
{

[[nodiscard]] fuse::Any ModuleFactory::construct(const std::filesystem::path &path) const {
  
}

[[nodiscard]] const std::type_info &ModuleFactory::type() const noexcept {
  return typeid(Module);
}

[[nodiscard]] ModuleFactory::VerificationResult ModuleFactory::verify(
    const std::filesystem::path &path) const noexcept {
  // First and foremost - validate the path
  if (!std::filesystem::is_directory(path)) {
    return VerificationResult::RootNotFound;
  }

  // Secondly, try to find a manifest
  const auto manifestPath = path / "manifest.json";
  if (!std::filesystem::exists(manifestPath)) {
    return VerificationResult::ManifestNotFound;
  }

  std::ifstream stream{manifestPath};
  if (!stream.is_open()) {
    return VerificationResult::ManifestNotFound;
  }

  Manifest manifest{nlohmann::json::parse(stream)};
  stream.close();

  // Check if the name field is present and not empty
  if (const auto name = manifest.name(); !name.has_value() || name->empty()) {
    return VerificationResult::NameMissing;
  }

  // And the version - we require both of them to identify the module
  if (!manifest.version().has_value()) {
    return VerificationResult::VersionMissing;
  }

  if (checkIntegrity_) {
    // Try to verify the integrity of the module if the hash exists.
    const auto hash = manifest.hash();
    if (hash.has_value()) {
      const auto actualHash = ModuleHash::compute(path);
      if (actualHash != hash.value()) {
        return VerificationResult::HashMismatch;
      }
    }
  }

  return VerificationResult::Verified;
}

}  // namespace introspect