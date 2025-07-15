#include <introspect/modules/ModuleFeatures.hpp>

namespace introspect
{

[[nodiscard]] ModuleFeatures ModuleFeatures::parse(const nlohmann::json& list) {
  constexpr auto k_maxRequirements{2u};

  // Drop this request if `list` is too large
  if (list.size() > k_maxRequirements) [[unlikely]] {
    spdlog::warn(
        "ModuleFeatures::parse: dropping off the request with a too large list "
        "size: `{}`",
        list.size());
    return {};
  }

  std::uint32_t val{Value::None};
  for (const std::string& req : list) {
    // Nothing better than this...
    if (req == "LocalFilesystem") {
      val |= Value::LocalFilesystem;
    } else if (req == "ProcessTree") {
      val |= Value::ProcessTree;
    } else {
      spdlog::warn("ModuleFeatures::parse: unknown requirement encountered: `{}`", req);
    }
  }

  return ModuleFeatures{std::move(static_cast<Value>(val))};
}

}  // namespace introspect