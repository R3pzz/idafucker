#include "NativeProcessEnvironment.hpp"

#include <bridge/win/native/TEB.hpp>
#include <bridge/win/process/Process.hpp>
#include <fuse/Enum.hpp>
#include <spdlog/spdlog.h>

namespace bridge::win
{

[[nodiscard]] Result<bool> Process::isDebuggerAttached() const noexcept {
  if (environment_ == nullptr) {
    return Status::makeBadHandle();
  }

  return environment_->processEnvironmentBlock(handle_).and_then(
      [this](void *pointer) -> Result<bool> {
        if (environment_->isEmulated().value()) {
          // Read process is different for a WoW64 process.
          return Status::makeNotImplemented();
        }

        PEB<std::uintptr_t> peb{pointer};
        return peb.readField(
            PEB<std::uintptr_t>::k_beingDebugged,
            [this](std::uintptr_t address, std::size_t size,
                   void *buffer) -> Status {
              SIZE_T numberOfBytesRead{};
              if (::ReadProcessMemory(
                      handle_, reinterpret_cast<LPCVOID>(address), buffer, size,
                      &numberOfBytesRead) == FALSE) {
                return Status::makeSystemSpecific(
                    "Call to `ReadProcessMemory`",
                    std::to_string(::GetLastError()));
              }

              return Status::makeSuccess();
            });
      });
}

[[nodiscard]] Result<bool> Process::is32Bit() const noexcept {
  if (environment_ == nullptr) {
    return Status::makeBadHandle();
  }

  return environment_->isEmulated();
}

[[nodiscard]] Result<bool> Process::is64Bit() const noexcept {
  if (environment_ == nullptr) {
    return Status::makeBadHandle();
  }

  return environment_->isNative();
}

[[nodiscard]] Result<std::filesystem::path> Process::executablePath()
    const noexcept {
  return Status::makeNotImplemented();
}

[[nodiscard]] std::unique_ptr<ProcessEnvironment> Process::allocateEnvironment(
    Handle handle) noexcept {
  if (handle == k_invalidHandle) {
    return nullptr;
  }

  BOOL isWoW64Process{};
  ::IsWow64Process(handle, &isWoW64Process);
  if (isWoW64Process) {
    return nullptr;
  }

  return std::make_unique<NativeProcessEnvironment>();
}

[[nodiscard]] Process::Handle Process::openProcessById(Id id,
                                                       Access access) noexcept {
  if (id == k_invalidId) {
    return k_invalidHandle;
  }

  // The `OpenProcess` function is not that unsafe to pass it throught the API
  // resolver.
  return ::OpenProcess(fuse::underlyingCast(access), FALSE, id);
}

}  // namespace bridge::win