#pragma once
#include <array>     // array
#include <memory>    // unique_ptr
#include <optional>  // optional
#include <span>      // span
#include <vector>    // vector

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/EnumeratedArray.hpp>
#include <idafucker/exceptions/Exception.hpp>

#include "Architecture.hpp"
#include "Instruction.hpp"
#include "Operand.hpp"

#include <Zydis/Decoder.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
enum class DecoderMode : std::uint8_t {
  MinimalMode,        //< *Undocumented*
  DecodeAmdBranches,  //< *Undocumented*
  EnableKNC,       //< 512-bit SIMD extension for `Knights Corner` architecture
  EnableMPX,       //< Intel Memory Protection Extension
  EnableCET,       //< Control-flow Enforcement Technology
  EnableLZCNT,     //< Leading Zero Count
  EnableTZCNT,     //< Trailing Zero Count
  EnableWBNOINVD,  //< Wait-for-No-Instruction-Fetch
  EnableCLDEMOTE,  //< Cache Line Demote
  IDAFUCKER_ARRAY_ENUMERATOR()
};

struct DecoderConfig final {
  EnumeratedArray<ZyanBool, DecoderMode> modes{false, false, false, true, true,
                                               true,  true,  false, true};
};

template <Architecture ISA> class Decoder final {
private:
  using ArchAdapter = detail::ZydisArchitectureAdapter<ISA>;

public:
  Decoder(const DecoderConfig &config)
      : state{new ZydisDecoder{
            ArchAdapter::MachMode, ArchAdapter::AddrWidth, config.modes}}
  {
  }

  ~Decoder() noexcept = default;

  [[nodiscard]] Instruction decode(const std::span<std::byte> &region) const
  {
    ZydisDecodedInstruction instr{};
    if (const auto result = ZydisDecoderDecodeBuffer(
            state.get(), region.data(), region.size(), &instr);
        result != ZYAN_STATUS_SUCCESS) {
      throw Exception{"ZydisDecoderDecodeBuffer failed with `"} << result
                                                                << "`";
    }

    return Instruction{instr};
  }

private:
  std::unique_ptr<ZydisDecoder> state{};

  IDAFUCKER_NONCOPYABLE(Decoder);
};
}  // namespace disasm

IDAFUCKER_NAMESPACE_END