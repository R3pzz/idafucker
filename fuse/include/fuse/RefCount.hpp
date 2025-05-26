#pragma once
#include <memory>  // shared_ptr

#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse
{
enum class ConcurrentModel {
  kNone,
  kAtomic,
};

namespace detail
{
template <typename Type, ConcurrentModel> struct select_concurrent_storage {};

template <typename Type>
struct select_concurrent_storage<Type, ConcurrentModel::kNone> {
  using type = Type;
};

template <typename Type>
struct select_concurrent_storage<Type, ConcurrentModel::kAtomic> {
  using type = std::atomic<Type>;
};

template <typename Type, ConcurrentModel Model>
using select_concurrent_storage_t =
    typename select_concurrent_storage<Type, Model>::type;
}  // namespace detail

template <ConcurrentModel Model = ConcurrentModel::kAtomic> class RefCountable {
public:
  constexpr RefCountable() noexcept = default;

  ~RefCountable() {
    if (refs > 0u)
      throw std::runtime_error{"object is alive"};
  }

  // We do not want to copy the ref count as technically we are spawning a new
  // object
  constexpr RefCountable(const RefCountable&) noexcept {}

  constexpr RefCountable& operator=(const RefCountable&) noexcept {}

  constexpr RefCountable(RefCountable&&) noexcept = default;
  constexpr RefCountable& operator=(RefCountable&) noexcept = default;

  std::size_t refAdded() const noexcept {
    return ++refs;
  }

  std::size_t refRemoved() const noexcept {
    return --refs;
  }

  std::size_t refCount() const noexcept {
    return refs;
  }

private:
  using ValueType = detail::select_concurrent_storage_t<std::size_t, Model>;
  mutable ValueType refs{};
};

template <typename T>
concept ref_countable = is_any_base_of_v<
    T, RefCountable<ConcurrentModel::kNone>,
    RefCountable<ConcurrentModel::kAtomic>>;

template <ref_countable T> class RCHandle {
public:
  constexpr RCHandle() noexcept = default;

  // Construct from a raw pointer
  explicit RCHandle(T* ref) noexcept : handle{ref} {
    if (handle != nullptr)
      handle->refAdded();
  }

  // Construct from another handle
  explicit RCHandle(const RCHandle& other) noexcept : handle{other.handle} {
    if (handle != nullptr)
      handle->refAdded();
  }

  // Construct from shared_ptr
  explicit RCHandle(const std::shared_ptr<T>& ref) noexcept
      : handle{ref.get()} {
    if (handle != nullptr)
      handle->refAdded();
  }

  // Construct from a nullptr
  explicit constexpr RCHandle(std::nullptr_t) noexcept : handle{nullptr} {}

  ~RCHandle() noexcept {
    handle->refRemoved();
  }

  [[nodiscard]] constexpr bool operator==(const RCHandle& rhs) const noexcept {
    return handle == rhs.handle;
  }

  [[nodiscard]] constexpr bool operator==(
      const std::shared_ptr<T>& rhs) const noexcept {
    return handle == rhs.get();
  }

  [[nodiscard]] constexpr bool operator==(std::nullptr_t) const noexcept {
    return handle == nullptr;
  }

  // Assign from a raw pointer
  auto& operator=(T* ref) noexcept {
    if (handle != nullptr)
      handle->refRemoved();

    handle = ref;
    if (handle != nullptr)
      handle->refAdded();

    return *this;
  }

  // Copy-assign from another handle
  auto& operator=(const RCHandle& rhs) noexcept {
    if (handle != nullptr)
      handle->refRemoved();

    handle = rhs.handle;
    if (handle != nullptr)
      handle->refAdded();

    return *this;
  }

  // Copy-assign from another handle
  auto& operator=(const std::shared_ptr<T>& rhs) noexcept {
    if (handle != nullptr)
      handle->refRemoved();

    handle = rhs.get();
    if (handle != nullptr)
      handle->refAdded();

    return *this;
  }

  // Assign from a nullptr
  auto& operator=(std::nullptr_t) noexcept {
    if (handle != nullptr)
      handle->refRemoved();
    handle = nullptr;
    return *this;
  }

  [[nodiscard]] constexpr T* operator->() const noexcept {
    return handle;
  }

  // Dereferencing
  [[nodiscard]] constexpr T& operator*() {
    if (handle == nullptr)
      throw std::runtime_error{"bad handle"};
    return *handle;
  }

  [[nodiscard]] constexpr const T& operator*() const {
    if (handle == nullptr)
      throw std::runtime_error{"bad handle"};
    return *handle;
  }

  // Raw pointer access
  [[nodiscard]] constexpr T* raw() noexcept {
    return handle;
  }

  [[nodiscard]] constexpr const T* raw() const noexcept {
    return handle;
  }

  // State managing
  void reset() noexcept {
    if (handle != nullptr)
      handle->refRemoved();
    handle = nullptr;
  }

private:
  T* handle{};
};

template <ref_countable T> RCHandle(T*) -> RCHandle<T>;
template <ref_countable T> RCHandle(std::shared_ptr<T>) -> RCHandle<T>;
}  // namespace fuse