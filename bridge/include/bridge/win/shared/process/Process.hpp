#pragma once
#include <filesystem>  // path
#include <memory>      // unique_ptr

#include <bridge/shared/Result.hpp>
#include <bridge/win/shared/pe_format/PortableExecutable.hpp>
#include <bridge/win/shared/process/ProcessEnvironment.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win
{

class Process final {
public:
  enum class Access : DWORD {
    None = 0u,
    AllAccess = PROCESS_ALL_ACCESS,
    CreateProc = PROCESS_CREATE_PROCESS,
    CreateThread = PROCESS_CREATE_THREAD,
    DuplicateHandle = PROCESS_DUP_HANDLE,
    MemoryAccess = PROCESS_VM_OPERATION,
    MemoryRead = PROCESS_VM_READ,
    MemoryWrite = PROCESS_VM_WRITE,
    QueryInformation = PROCESS_QUERY_INFORMATION,
    QueryLimitedInformation = PROCESS_QUERY_LIMITED_INFORMATION,
    SetInformation = PROCESS_SET_INFORMATION,
    SetQuota = PROCESS_SET_QUOTA,
    SuspendResume = PROCESS_SUSPEND_RESUME,
    Synchronize = SYNCHRONIZE,
    Terminate = PROCESS_TERMINATE,
  };

  // A type that represents a PID for a process. `DWORD` in Windows.
  using Id = DWORD;
  // Windows uses handles to access different process' data.
  using Handle = HANDLE;

  static constexpr Id k_invalidId{~0u};
  static constexpr Handle k_invalidHandle{nullptr};

  // Default constructor. Constructs an invalid process.
  constexpr Process() noexcept = default;

  // Constructs a process without getting its OS handle. Native operations
  // on this process will result in `ErrorCode::BadHandle` error.
  constexpr Process(Id id) noexcept : id_{std::move(id)} {}

  // Constructs a process out of a process ID (PID) and a corresponding handle.
  Process(Id id, Handle handle) noexcept
      : id_{std::move(id)},
        handle_{std::move(handle)},
        environment_{allocateEnvironment(handle)} {}

  // Constructs a process while also opening it with the desired access rights.
  Process(Id id, Access access) noexcept
      : id_{std::move(id)},
        handle_{openHandle(id, access)},
        environment_{allocateEnvironment(handle_)} {}

  // Compares two processes by their PIDs. This does not account for handles
  // since Windows can issue multiple handles for the same process.
  [[nodiscard]] constexpr bool operator==(const Process &rhs) const noexcept {
    return id_ == rhs.id_;
  }

  // Field getters ---

  // Returns the process ID (PID) of this process.
  [[nodiscard]] constexpr Id id() const noexcept {
    return id_;
  }

  // Native functions ---

  // Is a debugger attached to this process?
  [[nodiscard]] Result<bool> isDebuggerAttached() const noexcept;

  // Is this process emulated by WoW64?
  [[nodiscard]] Result<bool> is32Bit() const noexcept;

  // Is this process running under the native subsystem?
  [[nodiscard]] Result<bool> is64Bit() const noexcept;

  // Get the path to an executable that corresponds to this process.
  [[nodiscard]] Result<std::filesystem::path> path() const noexcept;

  [[nodiscard]] Result<>

private:
  [[nodiscard]] static Handle openHandle(Id id, Access access) noexcept;

  Id id_{k_invalidId};
  Handle handle_{k_invalidHandle};
};

}  // namespace bridge::win