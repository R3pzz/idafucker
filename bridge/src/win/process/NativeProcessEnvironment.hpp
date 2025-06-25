#pragma once
#include <bridge/win/process/ProcessEnvironment.hpp>

namespace bridge::win
{

class NativeProcessEnvironment final : public ProcessEnvironment {
public:
  constexpr NativeProcessEnvironment() noexcept = default;

  [[nodiscard]] Result<bool> isEmulated() const override;
  [[nodiscard]] Result<bool> isNative() const override;
  [[nodiscard]] Result<void *> processEnvironmentBlock(
      [[maybe_unused]] HANDLE) const override;
};

}  // namespace bridge::win