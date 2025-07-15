#pragma once
#include <any>     // any
#include <atomic>  // atomic_size_t
#include <memory>  // unique_ptr
#include <vector>  // vector

#include <introspect/Config.hpp>
#include <fuse/Any.hpp>
#include <fuse/Bits.hpp>
#include <fuse/Class.hpp>
#include <fuse/RefCount.hpp>

namespace introspect
{

class ResourceManager;

class Resource final : public fuse::RefCountable<> {
public:
  enum class Flags {
    None = 0u,
    Required = 1u << 0u,  //< The resource is required to stay in memory even if
                          // not referenced.
  };

  // Constructs the resource holder from an any-object
  Resource(ResourceManager& manager, const Flags flags, fuse::Any data) noexcept
      : manager{manager}, flags{flags}, data{std::move(data)} {}

  // Constructs the resource holder in-place
  template <class T>
  Resource([[maybe_unused]] std::in_place_type_t<T>,
           ResourceManager& manager,
           const Flags flags,
           auto&&... args) noexcept
      : manager{manager}, flags{flags}, data{T{std::forward<decltype(args)>(args)...}} {}

  ~Resource() = default;

  template <class T>
  [[nodiscard]] auto get() -> T* {
    return data.get<T>();
  }

  template <class T>
  [[nodiscard]] auto get() const -> const T* {
    return data.get<T>();
  }

  [[nodiscard]] const std::type_info& type() const noexcept {
    return data.type();
  }

  // Resource properties
  [[nodiscard]] constexpr bool empty() const noexcept {
    return data.empty();
  }

  [[nodiscard]] constexpr bool isRequired() const noexcept {
    return flags.contains(Flags::Required);
  }

  constexpr void markRequired() noexcept {
    flags.add(Flags::Required);
  }

private:
  void reset(fuse::Any&& to) noexcept {
    data = to;
  }

  ResourceManager& manager;
  fuse::Any data;               //< The data
  std::vector<Resource*> deps;  //< Dependencies
  fuse::Bits<Flags> flags;      //< Flags

  friend class ResourceManager;

  FUSE_NONCOPYABLE(Resource);
};

}  // namespace introspect