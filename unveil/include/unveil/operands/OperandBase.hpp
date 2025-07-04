#pragma once
#include <introspect/CoreDefines.hpp>
#include <introspect/base/BitSet.hpp>

#include "Fwd.hpp"

#include <Zydis/Zydis.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
// A signature of an operand contained in a single 32-bit wide variable.
// Defines some operand metadata and has some space for operand-specific
// data for the tightest and align-friendly encoding.
struct Signature {
  // The type of the operand.
  // |XXX-----|--------|--------|--------|
  static constexpr BitRange kType{0u, 2u};

  // Operand-specific metadata field.
  // |--------|--------|XXXXXXXX|XXXXXXXX|
  static constexpr BitRange kMeta{16u, 31u};

  constexpr Signature() noexcept = default;

  constexpr Signature(std::uint32_t data) noexcept : data{data} {}

  constexpr Signature &operator|=(auto rhs) noexcept {
    data |= static_cast<std::uint32_t>(rhs);
    return *this;
  }

  [[nodiscard]] constexpr std::uint16_t meta() const noexcept {
    return kMeta.read(data.get());
  }

  [[nodiscard]] static constexpr std::uint32_t writeOperandType(
      OperandType type) noexcept {
    return kType.write(static_cast<std::uint32_t>(type));
  }

  // Positions the `values` into the metadata slot and returns a signature with
  // them merged.
  [[nodiscard]] static constexpr std::uint32_t writeMeta(auto meta) noexcept {
    return kMeta.write(static_cast<std::uint16_t>(meta));
  }

  BitSet<std::uint32_t> data{};
};

// A base class for all possible operand types.
class Operand {
public:
  constexpr Operand() noexcept = default;

  constexpr Operand(Signature signature) noexcept : signature{signature} {}

  constexpr Operand(Signature signature, std::int64_t disp) noexcept
      : signature{signature}, value{disp} {}

protected:
  Signature signature{};
  union {
    std::int64_t memoryDisplacement{};
  } value{};
};
}  // namespace disasm

IDAFUCKER_NAMESPACE_END