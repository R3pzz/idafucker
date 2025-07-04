#pragma once
#include <memory>  // shared_ptr

#include <fuse/Config.hpp>
#include <fuse/TypeTraits.hpp>

namespace fuse
{
enum class ConcurrentModel {
  None,
  Atomic,
};

namespace detail
{
template <typename Type, ConcurrentModel>
struct select_concurrent_storage {};

template <typename Type>
struct select_concurrent_storage<Type, ConcurrentModel::None> {
  using type = Type;
};

template <typename Type>
struct select_concurrent_storage<Type, ConcurrentModel::Atomic> {
  using type = std::atomic<Type>;
};

template <typename Type, ConcurrentModel Model>
using select_concurrent_storage_t = typename select_concurrent_storage<Type, Model>::type;
}  // namespace detail

template <ConcurrentModel Model = ConcurrentModel::Atomic>
class RefCountable {
public:
  constexpr RefCountable() noexcept = default;

  ~RefCountable() noexcept(false) {
    if (refs_ > 0u)
      throw std::runtime_error{"object is alive"};
  }

  // We do not want to copy the ref count as technically we are spawning a new
  // object
  constexpr RefCountable(const RefCountable&) noexcept {}

  constexpr RefCountable& operator=(const RefCountable&) noexcept {}

  constexpr RefCountable(RefCountable&&) noexcept = default;
  constexpr RefCountable& operator=(RefCountable&) noexcept = default;

  std::size_t refAdded() const noexcept {
    return ++refs_;
  }

  std::size_t refRemoved() const noexcept {
    return --refs_;
  }

  std::size_t refCount() const noexcept {
    return refs_;
  }

private:
  using ValueType = detail::select_concurrent_storage_t<std::size_t, Model>;
  mutable ValueType refs_{};
};

namespace concepts
{

template <typename T>
concept ref_countable = one_of_bases<T,
                                     RefCountable<ConcurrentModel::None>,
                                     RefCountable<ConcurrentModel::Atomic>>;

}  // namespace concepts

template <concepts::ref_countable T>
class RCHandle {
public:
  constexpr RCHandle() noexcept = default;

  // Construct from a raw pointer
  explicit RCHandle(T* ref) noexcept : handle_{ref} {
    if (handle_ != nullptr)
      handle_->refAdded();
  }

  // Construct from another handle_
  explicit RCHandle(const RCHandle& other) noexcept : handle_{other.handle_} {
    if (handle_ != nullptr)
      handle_->refAdded();
  }

  // Construct from shared_ptr
  explicit RCHandle(const std::shared_ptr<T>& ref) noexcept : handle_{ref.get()} {
    if (handle_ != nullptr)
      handle_->refAdded();
  }

  // Construct from a nullptr
  explicit constexpr RCHandle(std::nullptr_t) noexcept : handle_{nullptr} {}

  ~RCHandle() noexcept {
    handle_->refRemoved();
  }

  [[nodiscard]] constexpr bool operator==(const RCHandle& rhs) const noexcept {
    return handle_ == rhs.handle_;
  }

  [[nodiscard]] constexpr bool operator==(const std::shared_ptr<T>& rhs) const noexcept {
    return handle_ == rhs.get();
  }

  [[nodiscard]] constexpr bool operator==(std::nullptr_t) const noexcept {
    return handle_ == nullptr;
  }

  // Assign from a raw pointer
  auto& operator=(T* ref) noexcept {
    if (handle_ != nullptr)
      handle_->refRemoved();

    handle_ = ref;
    if (handle_ != nullptr)
      handle_->refAdded();

    return *this;
  }

  // Copy-assign from another handle_
  auto& operator=(const RCHandle& rhs) noexcept {
    if (handle_ != nullptr)
      handle_->refRemoved();

    handle_ = rhs.handle_;
    if (handle_ != nullptr)
      handle_->refAdded();

    return *this;
  }

  // Copy-assign from another handle_
  auto& operator=(const std::shared_ptr<T>& rhs) noexcept {
    if (handle_ != nullptr)
      handle_->refRemoved();

    handle_ = rhs.get();
    if (handle_ != nullptr)
      handle_->refAdded();

    return *this;
  }

  // Assign from a nullptr
  auto& operator=(std::nullptr_t) noexcept {
    if (handle_ != nullptr)
      handle_->refRemoved();
    handle_ = nullptr;
    return *this;
  }

  [[nodiscard]] constexpr T* operator->() const noexcept {
    return handle_;
  }

  // Dereferencing
  [[nodiscard]] constexpr T& operator*() {
    if (handle_ == nullptr)
      throw std::runtime_error{"bad handle"};
    return *handle_;
  }

  [[nodiscard]] constexpr const T& operator*() const {
    if (handle_ == nullptr)
      throw std::runtime_error{"bad handle"};
    return *handle_;
  }

  // Raw pointer access
  [[nodiscard]] constexpr T* raw() noexcept {
    return handle_;
  }

  [[nodiscard]] constexpr const T* raw() const noexcept {
    return handle_;
  }

  // State managing
  void reset() noexcept {
    if (handle_ != nullptr)
      handle_->refRemoved();
    handle_ = nullptr;
  }

private:
  T* handle_{};
};

template <concepts::ref_countable T>
RCHandle(T*) -> RCHandle<T>;
template <concepts::ref_countable T>
RCHandle(std::shared_ptr<T>) -> RCHandle<T>;
}  // namespace fuse