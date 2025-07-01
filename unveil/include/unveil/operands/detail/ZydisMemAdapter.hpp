#pragma once
#include <introspect/CoreDefines.hpp>
#include <introspect/disasm/operands/Fwd.hpp>

#include <Zydis/Zydis.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm::detail
{

struct ZydisMemAdapter {
  [[nodiscard]] static constexpr AddressingMode parseMode(const ZydisDecodedOperand &op) noexcept {
    switch (op.encoding) {
    case ZYDIS_OPERAND_ENCODING_MODRM_REG:
    case ZYDIS_OPERAND_ENCODING_MODRM_RM:
      // ModR/M encodings are always indirect
      return AddressingMode::kIndirect;
    default:
      // We assume that Zydis has parsed the operands correctly, and if the
      // current operand's encoding is not ModR/M, it automatically becomes
      // a direct address.
      // NOTE: This may potentially lead to a bug if Zydis fucks up.
      return AddressingMode::kDirect;
    }
  }
};

} // namespace disasm::detail

IDAFUCKER_NAMESPACE_END