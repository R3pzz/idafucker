#pragma once
#include <vector> // vector

#include <unveil/Config.hpp>
#include <unveil/x86/Prefix.hpp>
#include <unveil/x86/Operand.hpp>

namespace unveil
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
  using Mnemonic = std::uint16_t;

  constexpr Instruction() noexcept = default;

  // Write this instruction in an assembly format.
  [[nodiscard]] std::string dumpAssembly() const noexcept;

private:
  Mnemonic mnemonic_{};       //< A mnemonic of the opcode of the instruction.
  std::vector<Prefix> prefixes_{};  //< Prefixes the instruction has.
  std::vector<Operand> operands_{};  //< Operands of the instruction.
};
}  // namespace unveil