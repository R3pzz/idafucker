#pragma once
#include <bridge/win/process/ProcessEnvironment.hpp>

namespace bridge::win
{

class WoW64ProcessEnvironment final : public ProcessEnvironment {
public:
  constexpr WoW64ProcessEnvironment() noexcept = default;

  [[nodiscard]] Result<bool> isEmulated() const override;
  [[nodiscard]] Result<bool> isNative() const override;
  [[nodiscard]] Result<void *> processEnvironmentBlock(
      [[maybe_unused]] HANDLE) const override;
};

}  // namespace bridge::win