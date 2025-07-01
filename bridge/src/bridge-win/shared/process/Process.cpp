#include <bridge-win/shared/process/Process.hpp>
#include <bridge-win/shared/structures/Peb.hpp>
#include <bridge-win/shared/syscalls/Syscalls.hpp>

namespace bridge::win::process
{

[[nodiscard]] StatusOr<Pid> Process::pid() const noexcept {
  // Skip for uninitialized processes
  if (data_.empty()) [[unlikely]] {
    return Status::makeBadHandle();
  }

  if (data_.isInHandleMode()) {
    // Get PID for handle and return if the PID is valid
    if (const auto id = ::GetProcessId(data_.handle()); id != k_badPid) {
      return id;
    }

    // Otherwise, make a system error
    return Status::makeOS();
  } else {
    // Get the PID directly
    return data_.pid();
  }
}

[[nodiscard]] StatusOr<bool> Process::hasDebugger() const noexcept {
  // Skip for processes that are not open
  if (data_.isInPIDMode()) [[unlikely]] {
    return Status::makeBadHandle();
  }

  // Ask the OS about this process
  PROCESS_BASIC_INFORMATION processInfo{};
  BRIDGE_WIN_CHECK_NT(syscalls::ntQueryInformationProcess(
      data_.handle(), ProcessBasicInformation, &processInfo,
      sizeof(PROCESS_BASIC_INFORMATION), nullptr));

  // Obtain the address of `BeingDebugged` field in the actual PEB
  const auto beingDebuggedAddress = structures::Peb::k_beingDebugged.on(
      processInfo.PebBaseAddress);

  // Read out the field
  BOOLEAN beingDebugged{};
  BRIDGE_WIN_CHECK_OS(::ReadProcessMemory(data_.handle(), beingDebuggedAddress,
                                          &beingDebugged, sizeof(BOOLEAN), nullptr));

  return beingDebugged;
}

}  // namespace bridge::win::process