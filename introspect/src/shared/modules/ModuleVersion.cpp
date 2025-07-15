#pragma once
#include <introspect/modules/ModuleVersion.hpp>

namespace introspect
{

[[nodiscard]] ModuleVersion ModuleVersion::parse(const nlohmann::json& object) {
  const std::regex versionRegex{R"(^(\d+)\.(\d+)\.(\d+)$)"};

  // Verify that the object is actually a string
  if (!object.is_string()) {
    return {};
  }

  // Try to match against the regex and parse out the version numbers
  std::smatch match{};
  const auto& versionString = object.get<std::string>();
  if (!std::regex_match(versionString, match, versionRegex)) {
    return {};
  }

  return ModuleVersion{std::stoul(match[1].str()), std::stoul(match[2].str()),
                       std::stoul(match[3].str())};
}

}  // namespace introspect