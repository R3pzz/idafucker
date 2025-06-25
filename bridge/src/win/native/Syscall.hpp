#pragma once
#include <array>  // array

#include <fuse/Enum.hpp>
#include <fuse/Platform.hpp>

#include <winternl.h>

#include "SyscallId.hpp"

// Allocate an execute-read-write section in the PE image
// to put the bytecode in.
#pragma section(".syscall_stub", execute, read)

namespace bridge::win
{

template <SyscallId Id, auto Index = fuse::underlyingCast(Id)>
class Syscall final {
public:
  constexpr Syscall() noexcept = default;

  [[nodiscard]] NTSTATUS operator()(auto &&...args) const noexcept {
    return call(std::forward<decltype(args)>(args)...);
  }

private:
  // The byte code was taken out straight from IDA(`ntdll.dll`).
  FUSE_SPECIFY_SECTION(".syscall_stub")
  static inline const std::array<std::uint8_t, 11u> k_syscallStub{
      0x4Cu, 0x8Bu, 0xD1u,                // `mov r10, rcx`
      0xB8u, Index, 0x00u, 0x00u, 0x00u,  // `mov eax, `Index``
      0x0Fu, 0x05u,                       // `syscall`
      0xC3u                               // `retn`
  };

  static DWORD FUSE_NOINLINE FUSE_STDCALL call(auto &&...args) noexcept {
    using Method = NTSTATUS(__stdcall)(decltype(args)...);

    return reinterpret_cast<Method *>(k_syscallStub.data())(
        std::forward<decltype(args)>(args)...);
  }
};

}  // namespace bridge::win