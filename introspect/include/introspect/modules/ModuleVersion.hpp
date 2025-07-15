#pragma once
#include <regex>   // regex
#include <string>  // string_view

#include <introspect/Config.hpp>

#include <nlohmann/json.hpp>

namespace introspect
{

class ModuleVersion final {
public:
  constexpr ModuleVersion() noexcept = default;
  constexpr ModuleVersion(std::uint32_t major,
                          std::uint32_t minor,
                          std::uint32_t patch) noexcept
      : major_{std::move(major)}, minor_{std::move(minor)}, patch_{std::move(patch)} {}

  [[nodiscard]] constexpr bool operator==(const ModuleVersion& rhs) const noexcept {
    return major_ == rhs.major_ && minor_ == rhs.minor_ && patch_ == rhs.patch_;
  }

  [[nodiscard]] constexpr bool operator!=(const ModuleVersion& rhs) const noexcept {
    return major_ != rhs.major_ || minor_ != rhs.minor_ || patch_ != rhs.patch_;
  }

  [[nodiscard]] constexpr std::uint32_t major() const noexcept {
    return major_;
  }

  [[nodiscard]] constexpr std::uint32_t minor() const noexcept {
    return minor_;
  }

  [[nodiscard]] constexpr std::uint32_t patch() const noexcept {
    return patch_;
  }

  [[nodiscard]] static ModuleVersion parse(const nlohmann::json& object);

private:
  std::uint32_t major_{};
  std::uint32_t minor_{};
  std::uint32_t patch_{};
};

}  // namespace introspect