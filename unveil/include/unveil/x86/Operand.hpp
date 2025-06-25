#pragma once
#include <fuse/Bits.hpp>
#include <unveil/Config.hpp>

namespace unveil
{

enum class OperandType {
  None,
  Mem,
  Imm,
  Reg,
};

// A signature of an operand contained in a single 32-bit wide variable.
// Defines some operand metadata and has some space for operand-specific
// data for the tightest and align-friendly encoding.
struct Signature {
  // |XXX-----|--------|--------|--------|
  static constexpr fuse::BitRange k_type{0u, 2u};  //< The type of the operand.
  // |--------|--------|XXXXXXXX|XXXXXXXX|
  static constexpr fuse::BitRange k_meta{
      16u, 31u};  //< Operand-specific metadata field.

  constexpr Signature() noexcept = default;

  constexpr Signature(std::uint32_t data) noexcept : data{data} {}

  constexpr Signature &operator|=(auto rhs) noexcept {
    data |= static_cast<std::uint32_t>(rhs);
    return *this;
  }

  [[nodiscard]] constexpr OperandType type() const noexcept {
    return static_cast<OperandType>(k_type.fetchFrom(data.get()));
  }

  [[nodiscard]] constexpr std::uint16_t meta() const noexcept {
    return k_meta.fetchFrom(data.get());
  }

  [[nodiscard]] static constexpr std::uint32_t writeOperandType(
      OperandType type) noexcept {
    return k_type.storeTo(static_cast<std::uint32_t>(type));
  }

  // Positions the `values` into the metadata slot and returns a signature with
  // them merged.
  [[nodiscard]] static constexpr std::uint32_t writeMeta(auto meta) noexcept {
    return k_meta.storeTo(static_cast<std::uint16_t>(meta));
  }

  fuse::Bits<std::uint32_t> data{};
};

// A base class for all possible operand types.
class Operand {
public:
  constexpr Operand() noexcept = default;

  constexpr Operand(Signature signature) noexcept : sig_{signature} {}

  constexpr Operand(Signature signature, std::int64_t disp) noexcept
      : sig_{signature}, value_{disp} {}

  [[nodiscard]] constexpr bool isMem() const noexcept {
    return sig_.type() == OperandType::Mem;
  }

protected:
  Signature sig_{};

  union {
    std::int64_t disp{};
  } value_{};
};

enum class AddressMode : std::uint32_t {
  None,
  Direct,
  Indirect,
};

class Reg : public Operand {
public:
  [[nodiscard]] constexpr std::uint8_t id() const noexcept {
    return 0u;
  }
};

struct MemEncoding {
  // Memory map: |XXX-----|--------|
  static constexpr fuse::BitRange k_baseReg{0u, 2u};
  // Memory map: |---XXX--|--------|
  static constexpr fuse::BitRange k_indexReg{3u, 5u};
  // Memory map: |------XX|--------|
  static constexpr fuse::BitRange k_scale{6u, 7u};
  // Memory map: |--------|XXX-----|
  static constexpr fuse::BitRange k_segmentReg{8u, 10u};
  // Memory map: |--------|---XX---|
  static constexpr fuse::BitRange k_mode{11u, 12u};

  [[nodiscard]] static constexpr std::uint16_t encodeImmediate() noexcept {
    return k_mode.storeTo(static_cast<std::uint16_t>(AddressMode::Direct));
  }

  [[nodiscard]] static constexpr std::uint16_t encodeDisp(
      const Reg &baseReg) noexcept {
    return k_mode.storeTo(static_cast<std::uint16_t>(AddressMode::Indirect)) |
           k_baseReg.storeTo(static_cast<std::uint16_t>(baseReg.id()));
  }

  [[nodiscard]] static constexpr std::uint16_t encodeSIB(
      const Reg &baseReg,
      const Reg &indexReg,
      std::uint8_t scale) noexcept {
    return k_mode.storeTo(static_cast<std::uint16_t>(AddressMode::Indirect)) |
           k_baseReg.storeTo(static_cast<std::uint16_t>(baseReg.id())) |
           k_indexReg.storeTo(static_cast<std::uint16_t>(indexReg.id())) |
           k_scale.storeTo(static_cast<std::uint16_t>(scale));
  }
};

class Mem : public Operand {
public:
  constexpr Mem() noexcept
      : Operand{Signature::writeOperandType(OperandType::Mem)} {}

  // Memory address is loaded in as an immediate value:
  // `direct`:`0x1000`
  constexpr Mem(std::int64_t disp) noexcept
      : Operand{Signature::writeOperandType(OperandType::Mem) |
                    Signature::writeMeta(MemEncoding::encodeImmediate()),
                disp} {}

  // Memory address calculated via a register and a displacement:
  // [`baseReg` + `disp`]
  constexpr Mem(const Reg &base, std::int64_t disp) noexcept
      : Operand{Signature::writeOperandType(OperandType::Mem) |
                    Signature::writeMeta(MemEncoding::encodeDisp(base)),
                disp} {}

  // Memory address calculated via a register, an index,
  // a scale and a displacement:
  // [`baseReg` + `indexReg` * `scale` + `disp`]
  constexpr Mem(const Reg &base,
                const Reg &index,
                std::uint8_t scale,
                std::int64_t disp) noexcept
      : Operand{Signature::writeOperandType(OperandType::Mem) |
                    Signature::writeMeta(
                        MemEncoding::encodeSIB(base, index, scale)),
                disp} {}

  [[nodiscard]] constexpr std::int64_t disp() const noexcept {
    return value_.disp;
  }

  [[nodiscard]] constexpr std::uint8_t scale() const noexcept {
    // The scale is mapped like 00 -> 1; 01 -> 2; 10 -> 4; 11 -> 8
    const auto scaleAsPowerOfTwo = MemEncoding::k_scale.fetchFrom(sig_.meta());
    return 1u << scaleAsPowerOfTwo;
  }

  [[nodiscard]] constexpr AddressMode mode() const noexcept {
    return static_cast<AddressMode>(MemEncoding::k_mode.fetchFrom(sig_.meta()));
  }
};

class Imm : public Operand {
public:
  constexpr Imm() noexcept = default;

private:
  union {
    float floating{};
    int integer;
  } value_{};
};

}  // namespace unveil