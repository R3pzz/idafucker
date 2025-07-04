#pragma once
#include <introspect/CoreDefines.hpp>

#include "OperandBase.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{

enum class RegId : std::uint16_t {};

class Reg : public Operand {
public:
  [[nodiscard]] constexpr std::uint8_t id() const noexcept {
    return 0u;
  }

private:
  RegId id{};
};

}  // namespace disasm

IDAFUCKER_NAMESPACE_END