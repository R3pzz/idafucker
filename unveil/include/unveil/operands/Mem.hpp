#pragma once
#include <introspect/CoreDefines.hpp>
#include <introspect/exceptions/Exception.hpp>

#include "Reg.hpp"
#include "detail/ZydisMemAdapter.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
struct MemEncoding {
  // Memory map: |XXX-----|--------|
  static constexpr BitRange kBaseReg{0u, 2u};
  // Memory map: |---XXX--|--------|
  static constexpr BitRange kIndexReg{3u, 5u};
  // Memory map: |------XX|--------|
  static constexpr BitRange kScale{6u, 7u};
  // Memory map: |--------|XXX-----|
  static constexpr BitRange kSegmentReg{8u, 10u};
  // Memory map: |--------|---XX---|
  static constexpr BitRange kMode{11u, 12u};

  [[nodiscard]] static constexpr std::uint16_t encodeImmediate() noexcept {
    return kMode.write(static_cast<std::uint16_t>(AddressingMode::kDirect));
  }

  [[nodiscard]] static constexpr std::uint16_t encodeDisp(
      const Reg &baseReg) noexcept {
    return kMode.write(static_cast<std::uint16_t>(AddressingMode::kIndirect)) |
           kBaseReg.write(static_cast<std::uint16_t>(baseReg.id()));
  }

  [[nodiscard]] static constexpr std::uint16_t encodeSIB(
      const Reg &baseReg, const Reg &indexReg, std::uint8_t scale) noexcept {
    return kMode.write(static_cast<std::uint16_t>(AddressingMode::kIndirect)) |
           kBaseReg.write(static_cast<std::uint16_t>(baseReg.id())) |
           kIndexReg.write(static_cast<std::uint16_t>(indexReg.id())) |
           kScale.write(static_cast<std::uint16_t>(scale));
  }

  [[nodiscard]] static constexpr std::uint16_t encodeZydis(
      const ZydisDecodedOperand &op) {
    if (op.type != ZYDIS_OPERAND_TYPE_MEMORY) [[unlikely]]
      throw Exception{"Expected a memory operand, got `"} << op.type << "`";

    const auto mode = detail::ZydisMemAdapter::parseMode(op);
    return kMode.write(static_cast<std::uint16_t>(mode)) &
           encodeSIB({op.mem.base}, {op.mem.index}, op.mem.scale);
  }
};

class Mem : public Operand {
public:
  constexpr Mem() noexcept
      : Operand{Signature::writeOperandType(OperandType::kMem)} {}

  // Memory address is loaded in as an immediate value:
  // `direct`:`0x1000`
  constexpr Mem(std::int64_t disp) noexcept
      : Operand{
            Signature::writeOperandType(OperandType::kMem) |
                Signature::writeMeta(MemEncoding::encodeImmediate()),
            disp} {}

  // Memory address calculated via a register and a displacement:
  // [`baseReg` + `disp`]
  constexpr Mem(const Reg &base, std::int64_t disp) noexcept
      : Operand{
            Signature::writeOperandType(OperandType::kMem) |
                Signature::writeMeta(MemEncoding::encodeDisp(base)),
            disp} {}

  // Memory address calculated via a register, an index,
  // a scale and a displacement:
  // [`baseReg` + `indexReg` * `scale` + `disp`]
  constexpr Mem(
      const Reg &base, const Reg &index, std::uint8_t scale,
      std::int64_t disp) noexcept
      : Operand{
            Signature::writeOperandType(OperandType::kMem) |
                Signature::writeMeta(
                    MemEncoding::encodeSIB(base, index, scale)),
            disp} {}

  // Decode a Zydis operand and construct a `Mem` object off of it.
  constexpr Mem(const ZydisDecodedOperand &op)
      : Operand{
            Signature::writeOperandType(OperandType::kMem) |
                Signature::writeMeta(MemEncoding::encodeZydis(op)),
            op.mem.disp.value} {}

  [[nodiscard]] constexpr std::int64_t disp() const noexcept {
    return value.memoryDisplacement;
  }

  [[nodiscard]] constexpr AddressingMode mode() const noexcept {
    return static_cast<AddressingMode>(
        MemEncoding::kMode.read(signature.meta()));
  }
};
}  // namespace disasm

IDAFUCKER_NAMESPACE_END