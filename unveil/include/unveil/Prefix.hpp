#pragma once
#include <unveil/Config.hpp>
#include <fuse/Bits.hpp>

#include <Zydis/Zydis.h>

IDAFUCKER_NAMESPACE_BEGIN

namespace disasm
{
enum class PrefixGroup : std::uint8_t {
  None,  //< *undefined*
  REX,   //< REX prefix group.
  XOP,   //< XOP prefix group (vector extensions).
  VEX,   //< VEX prefix group (vector extensions).
  EVEX,  //< EVEX prefix group (vector extensions).
  MVEX,  //< MVEX prefix group (vector extensions).
};

struct PrefixEncoding {
  using WordSize = std::uint32_t;

  // Memory map: |XXX-----|--------|--------|--------|
  static constexpr BitRange k_group{0u, 2u};  //< Related prefix group.
  // Memory map: |---XXXXX|--------|--------|--------|
  static constexpr BitRange k_relativePos{
      3u, 7u};  //< Prefix byte position relative to the instruction start.
  // Memory map: |--------|XXXXXXXX|XXXXXXXX|XXXXXXXX|
  static constexpr BitRange k_meta{8u, 31u};  //< Group-specific prefix meta.

  [[nodiscard]] static constexpr WordSize encodeGroup(
      PrefixGroup group) noexcept {
    return k_group.write(static_cast<WordSize>(group));
  }

  [[nodiscard]] static constexpr WordSize encodeRelativePos(
      std::uint8_t relativePos) noexcept {
    return k_relativePos.write(static_cast<WordSize>(relativePos));
  }

  [[nodiscard]] static constexpr WordSize encodeMeta(WordSize meta) noexcept {
    return k_meta.write(static_cast<WordSize>(meta));
  }
};

class Prefix {
public:
  static constexpr auto k_undefinedPos{0b11111u};

  // Initialize a completely blank `Prefix` object.
  constexpr Prefix() noexcept = default;

  // Initialize a group-specific `Prefix` with known parameters.
  constexpr Prefix(
      PrefixGroup group, std::uint8_t relativePos, std::uint32_t meta) noexcept
      : signature{
            PrefixEncoding::encodeGroup(group) |
            PrefixEncoding::encodeRelativePos(relativePos) |
            PrefixEncoding::encodeMeta(meta)} {}

  // Upcast this object to a prefix specification.
  template <typename T> [[nodiscard]] constexpr auto as() noexcept -> T& {
    return static_cast<Prefix&>(*this);
  }

  template <typename T>
  [[nodiscard]] constexpr auto as() const noexcept -> const T& {
    return static_cast<const Prefix&>(*this);
  }

  [[nodiscard]] constexpr PrefixGroup group() const noexcept {
    return static_cast<PrefixGroup>(signature.read(PrefixEncoding::kGroup));
  }

  [[nodiscard]] constexpr std::uint8_t relativePos() const noexcept {
    return static_cast<std::uint8_t>(
        signature.read(PrefixEncoding::kRelativePos));
  }

  [[nodiscard]] constexpr bool valid() const noexcept {
    return group() != PrefixGroup::kNone;
  }

  [[nodiscard]] constexpr bool isREX() const noexcept {
    return group() == PrefixGroup::kREX;
  }

  [[nodiscard]] constexpr bool isXOP() const noexcept {
    return group() == PrefixGroup::kXOP;
  }

  [[nodiscard]] constexpr bool isVEX() const noexcept {
    return group() == PrefixGroup::kVEX;
  }

  [[nodiscard]] constexpr bool isEVEX() const noexcept {
    return group() == PrefixGroup::kEVEX;
  }

  [[nodiscard]] constexpr bool isMVEX() const noexcept {
    return group() == PrefixGroup::kMVEX;
  }

protected:
  [[nodiscard]] constexpr std::uint32_t meta() const noexcept {
    return signature.read(PrefixEncoding::kMeta);
  }

private:
  BitSet<std::uint32_t> signature{};
};

class REXPrefix : public Prefix {
public:
  enum class Type : std::uint8_t {
    kB,
    kR,
    kX,
    kW,
  };

  constexpr REXPrefix() noexcept
      : Prefix{PrefixGroup::kREX, Prefix::kUndefinedPos, 0u} {}

  constexpr REXPrefix(Type type, std::uint8_t relativePos) noexcept
      : Prefix{
            PrefixGroup::kREX, relativePos, static_cast<std::uint32_t>(type)} {}

  [[nodiscard]] constexpr Type type() const noexcept {
    return static_cast<Type>(meta());
  }
};
}  // namespace disasm

IDAFUCKER_NAMESPACE_END