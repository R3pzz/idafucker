#pragma once
#include <idafucker/CoreDefines.hpp>

#include <Zydis/Zydis.h>

#include "operands/Mem.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
// Instruction is layed out like:
// [prefix] [opcode] [operand 1], [operand 2], ...
//
// Each part consists of:
//
//  * prefix:
//    - group: Lock/Repeat, Seg override,
//             Op-size override, Addr-size override, REX;
//    - variant(for example, GS, CS, FS for seg override, REX.W, REX.B for REX).
//  * opcode:
//    - opcode: the opcode itself;
//    - CPU flags: the flags affected after executing the operation;
//    - operand count: how many operands the operation accepts;
//    - jump length: short, near, far.



class Instruction final {
public:
  // According to ZydisMnemonic::ZYDIS_MNEMONIC_REQUIRED_BITS, at least
  // 11 bits bits are requires to represent all possible mnemonics.
  using Mnemonic = std::uint16_t;

  constexpr Instruction() noexcept = default;

  constexpr Instruction(const ZydisDecodedInstruction &instr) noexcept
      : mnemonic_{instr.mnemonic}, prefixes_{}

private:
  Mnemonic mnemonic_{};       //< A mnemonic of the opcode of the instruction.
  BitSet<Prefix> prefixes_{};  //< Prefixes the instruction has.
  std::vector<Operand> operands_{};  //< Operands of the instruction.
};
}  // namespace disasm

IDAFUCKER_NAMESPACE_END