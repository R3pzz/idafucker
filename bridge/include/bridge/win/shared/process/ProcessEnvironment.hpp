#pragma once
#include <bridge/shared/Result.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win
{

class ProcessEnvironment {
public:
  virtual ~ProcessEnvironment() noexcept = default;

  // Checks if the process is ran under a WoW64 emulator.
  [[nodiscard]] virtual Result<bool> isEmulated() const = 0;
  // Checks if the process is being executed natively.
  [[nodiscard]] virtual Result<bool> isNative() const = 0;

  [[nodiscard]] virtual Result<void *> processEnvironmentBlock(
      HANDLE handle) const = 0;
};

}  // namespace bridge::win