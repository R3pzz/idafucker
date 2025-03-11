#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/templates/BitFlags.hpp>
#include "ResourceType.hpp"

#include <atomic> // atomic_size_t
#include <memory> // unique_ptr

IDAFUCKER_NAMESPACE_BEGIN

class Resource {
public:
  enum class Flags {
    None = 0u,
    Required = 1u << 0u, //< The resource is required to stay in memory even if not referenced.
    Cached = 1u << 1u, //< The resource is loaded.
  };

public:
  constexpr Resource() noexcept = default;
  constexpr Resource(BitFlags<Flags> flags) noexcept : _flags{flags} {}
  
  virtual ~Resource() noexcept = default;

  // Resource identification
  [[nodiscard]] virtual ResourceType type() const noexcept = 0;

  // Reference counting
  auto onReferenceAdded() const { return ++_referenceCount; }
  auto onReferenceRemoved() const { return --_referenceCount; }

  [[nodiscard]] bool isReferenced() const { return _referenceCount != 0u; }
  [[nodiscard]] auto referenceCount() const { return _referenceCount.load(); }

  // Resource properties
  [[nodiscard]] constexpr bool isRequired() const noexcept { return _flags.contains(Flags::Required); }
  [[nodiscard]] constexpr bool isCached() const noexcept { return _flags.contains(Flags::Cached); }

  constexpr void markRequired() noexcept { _flags.add(Flags::Required); }

  // A utility function - REPLACEME
  [[nodiscard]] bool shouldUncache() const noexcept { return !isReferenced() && !_flags.contains(Flags::Required); }

  // Bring the resource in/out of memory
  // Todos:
  //  * Return an error state instead of bool for debugging/user experience
  virtual bool cache() = 0;
  virtual bool uncache() = 0;

protected:
  BitFlags<Flags> _flags{};

private:
  mutable std::atomic_size_t _referenceCount{};
};

IDAFUCKER_NAMESPACE_END