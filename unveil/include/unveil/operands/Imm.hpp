#pragma once
#include <idafucker/CoreDefines.hpp>

#include "OperandBase.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
// An immediate operand. Represented as hex or as a floating-point value.
// Can be assigned from any value whose size is lower than or equal to
// `Largest`.
class Imm : public Operand {
public:
  // The largest size the operand can be. 32-bit on x86 and 64-bit on x86_64.
  // However, 64-bit operand size requires a special instruction - `movabs`.
  using Largest = std::uintptr_t;

private:
  Largest value{};
};
}  // namespace disasm

IDAFUCKER_NAMESPACE_END