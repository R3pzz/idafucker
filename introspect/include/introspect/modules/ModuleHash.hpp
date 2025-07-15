#pragma once
#include <filesystem> // path
#include <string> // string

#include <introspect/Config.hpp>

namespace introspect
{

class ModuleHash final {
public:
  constexpr ModuleHash() noexcept = default;
  constexpr ModuleHash(std::string value) noexcept : value_{std::move(value)} {}

  [[nodiscard]] bool operator==(const ModuleHash &rhs) const noexcept {
    return value_ == rhs.value_;
  }
  
  [[nodiscard]] bool operator!=(const ModuleHash &rhs) const noexcept {
    return value_ != rhs.value_;
  }

  [[nodiscard]] constexpr bool empty() const noexcept {
    return value_.empty();
  }

  [[nodiscard]] constexpr const std::string &value() const noexcept {
    return value_;
  }

  [[nodiscard]] static ModuleHash compute(const std::filesystem::path &path);

private:
  std::string value_{};
};

}  // namespace introspect