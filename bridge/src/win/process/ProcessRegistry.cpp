#include "../native/SyscallDefinitions.hpp"

#include <bridge/win/process/ProcessRegistry.hpp>

#include <TlHelp32.h>

namespace bridge::win
{

[[nodiscard]] Process ProcessRegistry::currentProcess() {
  return Process{::GetCurrentProcessId(), ::GetCurrentProcess()};
}

[[nodiscard]] Result<Process> ProcessRegistry::processByName(
    const std::wstring &name,
    const Process::Access access) {
  if (name.empty()) [[unlikely]] {
    return Status::makeInvalidArgument();
  }

  auto snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0u);
  if (snapshot == NULL) {
    return Status::makeUnknown("CreateToolhelp32Snapshot failed");
  }

  PROCESSENTRY32W processEntry{.dwSize = sizeof(PROCESSENTRY32W)};
  for (auto success = ::Process32FirstW(snapshot, &processEntry);
       success != FALSE; success = ::Process32NextW(snapshot, &processEntry)) {
    if (processEntry.szExeFile == name) {
      return Process{processEntry.th32ProcessID, std::move(access)};
    }
  }

  return Status::makeNotFound();
}

}  // namespace bridge::win