#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
enum class OperandType : std::uint32_t {
  kMem = 0u,
  kImm,
  kReg,
  kLabel,
};

enum class AddressingMode : std::uint8_t {
  kNone = 0x0u,
  kDirect = 1u << 0u,
  kIndirect = 1u << 1u,
};
} // namespace disasm

IDAFUCKER_NAMESPACE_END