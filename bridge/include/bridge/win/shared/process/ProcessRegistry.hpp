#pragma once
#include <bridge/win/process/Process.hpp>
#include <fuse/Class.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win
{

class ProcessRegistry final {
public:
  [[nodiscard]] static Process currentProcess();

  // Tries to find a process by its name.
  [[nodiscard]] static Result<Process> processByName(
      const std::wstring &name,
      const Process::Access access);
};

}  // namespace bridge::win