#pragma once
#include <span> // span

#include <fuse/Bits.hpp>
#include <unveil/Config.hpp>

namespace unveil
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
  static constexpr fuse::BitRange k_group{0u, 2u};  //< Related prefix group.
  // Memory map: |---XXXXX|--------|--------|--------|
  static constexpr fuse::BitRange k_relativePos{
      3u, 7u};  //< Prefix byte position relative to the instruction start.
  // Memory map: |--------|XXXXXXXX|XXXXXXXX|XXXXXXXX|
  static constexpr fuse::BitRange k_meta{8u,
                                         31u};  //< Group-specific prefix meta.

  [[nodiscard]] static constexpr WordSize encodeGroup(
      PrefixGroup group) noexcept {
    return k_group.storeTo(static_cast<WordSize>(group));
  }

  [[nodiscard]] static constexpr WordSize encodeRelativePos(
      std::uint8_t relativePos) noexcept {
    return k_relativePos.storeTo(static_cast<WordSize>(relativePos));
  }

  [[nodiscard]] static constexpr WordSize encodeMeta(WordSize meta) noexcept {
    return k_meta.storeTo(static_cast<WordSize>(meta));
  }
};

class Prefix {
public:
  static constexpr auto k_undefinedPos{0b11111u};

  // Initialize a completely blank `Prefix` object.
  constexpr Prefix() noexcept = default;

  // Initialize a group-specific `Prefix` with known parameters.
  constexpr Prefix(PrefixGroup group,
                   std::uint8_t relativePos,
                   std::uint32_t meta) noexcept
      : sig_{PrefixEncoding::encodeGroup(group) |
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
    return static_cast<PrefixGroup>(
        PrefixEncoding::k_group.fetchFrom(sig_).get());
  }

  [[nodiscard]] constexpr std::uint8_t relativePos() const noexcept {
    return static_cast<std::uint8_t>(
        sig_.read(PrefixEncoding::k_relativePos.fetchFrom(sig_).get()));
  }

  [[nodiscard]] constexpr bool valid() const noexcept {
    return group() != PrefixGroup::None;
  }

protected:
  [[nodiscard]] constexpr std::uint32_t meta() const noexcept {
    return PrefixEncoding::k_meta.fetchFrom(sig_);
  }

private:
  fuse::Bits<std::uint32_t> sig_{};
};

// Represents any of the REX... prefixes.
class REX : public Prefix {
public:
  enum class Type : std::uint8_t { B, R, X, W, };

  constexpr REX() noexcept
      : Prefix{PrefixGroup::REX, k_undefinedPos, 0u} {}

  constexpr REX(Type type, std::uint8_t relativePos) noexcept
      : Prefix{PrefixGroup::REX, relativePos,
               static_cast<std::uint32_t>(type)} {}

  [[nodiscard]] constexpr Type type() const noexcept {
    return static_cast<Type>(meta());
  }
};

class OrderedPrefixView {
public:
  // Construct the view from two contiguous iterators.
  template <std::contiguous_iterator ContiguousIterator>
  constexpr OrderedPrefixView(ContiguousIterator &&begin, ContiguousIterator &&end) noexcept
      : prefixes_{std::forward<ContiguousIterator>(begin), std::forward<ContiguousIterator>(end)} {}

  // Print out only effective prefixes the prefix list has.
  [[nodiscard]] std::string dumpEffective() const;

private:
  std::span<Prefix> prefixes_;
}

}  // namespace unveil