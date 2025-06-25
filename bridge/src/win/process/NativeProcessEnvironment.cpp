#include "NativeProcessEnvironment.hpp"

#include "../native/SyscallDefinitions.hpp"

namespace bridge::win
{

[[nodiscard]] Result<bool> NativeProcessEnvironment::isEmulated() const {
  return false;
}

[[nodiscard]] Result<bool> NativeProcessEnvironment::isNative() const {
  return true;
}

[[nodiscard]] Result<void *> NativeProcessEnvironment::processEnvironmentBlock(
    HANDLE handle) const {
  DWORD bytesRead{};
  PROCESS_BASIC_INFORMATION processBasicInformation{};
  const auto ntStatus =
      ntQueryInformationProcess(handle, ProcessBasicInformation, &processBasicInformation,
                                sizeof(PROCESS_BASIC_INFORMATION), &bytesRead);
  if (NT_ERROR(ntStatus)) {
    return Status::makeSystemSpecific("Call to `NtQueryInformationProcess`",
                                      std::to_string(ntStatus));
  }

  return processBasicInformation.PebBaseAddress;
}

}  // namespace bridge::win