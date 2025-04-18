#pragma once
#include <memory>  // shared_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/exceptions/Exception.hpp>

//
// An Intrusive Handle is a auto-ref-counted handle to an object,
// which should provide such members:
//
//  * refAdded() -> increments object's reference count;
//  * refRemoved() -> decrements object's reference count.
//

IDAFUCKER_NAMESPACE_BEGIN

enum class ConcurrentModel {
  None,
  Atomic,
};

namespace detail
{
template <typename Type, ConcurrentModel> struct select_concurrent_storage {};

template <typename Type>
struct select_concurrent_storage<Type, ConcurrentModel::None> {
  using type = Type;
};

template <typename Type>
struct select_concurrent_storage<Type, ConcurrentModel::Atomic> {
  using type = std::atomic<Type>;
};

template <typename Type, ConcurrentModel Model>
using select_concurrent_storage_t =
    typename select_concurrent_storage<Type, Model>::type;
}  // namespace detail

template <ConcurrentModel Model = ConcurrentModel::Atomic> class RefCountable {
public:
  constexpr RefCountable() noexcept = default;

  ~RefCountable()
  {
    if (refCount_ > 0u)
      throw Exception{"ValueContainer destroyed while still referenced"};
  }

  // We do not want to copy the ref count as technically we are spawning a new
  // object
  constexpr RefCountable(const RefCountable&) noexcept {}

  constexpr RefCountable& operator=(const RefCountable&) noexcept {}

  constexpr RefCountable(RefCountable&&) noexcept = default;
  constexpr RefCountable& operator=(RefCountable&) noexcept = default;

  std::size_t refAdded() const noexcept
  {
    return ++refCount_;
  }

  std::size_t refRemoved() const noexcept
  {
    return --refCount_;
  }

  std::size_t refCount() const noexcept
  {
    return refCount_;
  }

private:
  using ValueType = detail::select_concurrent_storage_t<std::size_t, Model>;

protected:
  mutable ValueType refCount_{};
};

template <typename T>
concept ref_countable = is_any_base_of_v<
    T, RefCountable<ConcurrentModel::None>,
    RefCountable<ConcurrentModel::Atomic>>;

template <ref_countable T> class RcHandle {
public:
  constexpr RcHandle() noexcept = default;

  // Construct from a raw pointer
  explicit RcHandle(T* ref) noexcept : handle_{ref}
  {
    if (handle_ != nullptr)
      handle_->refAdded();
  }

  // Construct from another handle
  explicit RcHandle(const RcHandle& other) noexcept : handle_{other.handle_}
  {
    if (handle_ != nullptr)
      handle_->refAdded();
  }

  // Construct from a nullptr
  explicit constexpr RcHandle(std::nullptr_t) noexcept : handle_{nullptr} {}

  // Construct from shared_ptr
  explicit constexpr RcHandle(const std::shared_ptr<T>& ref) noexcept
      : handle_{ref.get()}
  {
    if (handle_ != nullptr)
      handle_->refAdded();
  }

  ~RcHandle() noexcept
  {
    handle_->refRemoved();
  }

  [[nodiscard]] constexpr bool operator==(std::nullptr_t) const noexcept
  {
    return handle_ == nullptr;
  }

  // Assign from a raw pointer
  auto& operator=(T* ref) noexcept
  {
    if (handle_ != nullptr)
      handle_->refRemoved();

    handle_ = ref;
    if (handle_ != nullptr)
      handle_->refAdded();

    return *this;
  }

  // Copy-assign from another handle
  auto& operator=(const RcHandle& rhs) noexcept
  {
    if (handle_ != nullptr)
      handle_->refRemoved();

    handle_ = rhs.handle_;
    if (handle_ != nullptr)
      handle_->refAdded();

    return *this;
  }

  // Copy-assign from another handle
  auto& operator=(const std::shared_ptr<T>& rhs) noexcept
  {
    if (handle_ != nullptr)
      handle_->refRemoved();

    handle_ = rhs.get();
    if (handle_ != nullptr)
      handle_->refAdded();

    return *this;
  }

  // Assign from a nullptr
  auto& operator=(std::nullptr_t) noexcept
  {
    if (handle_ != nullptr)
      handle_->refRemoved();
    handle_ = nullptr;
    return *this;
  }

  [[nodiscard]] constexpr T* operator->() const noexcept
  {
    return handle_;
  }

  // Dereferencing
  [[nodiscard]] constexpr T& operator*()
  {
    if (handle_ == nullptr)
      throw Exception{"Bad rc handle access"};
    return *handle_;
  }

  [[nodiscard]] constexpr const T& operator*() const
  {
    if (handle_ == nullptr)
      throw Exception{"Bad rc handle access"};
    return *handle_;
  }

  // Raw pointer access
  [[nodiscard]] constexpr T* raw() noexcept
  {
    return handle_;
  }

  [[nodiscard]] constexpr const T* raw() const noexcept
  {
    return handle_;
  }

  // State managing
  void reset() noexcept
  {
    if (handle_ != nullptr)
      handle_->refRemoved();
    handle_ = nullptr;
  }

private:
  T* handle_{};
};

template <ref_countable T> RcHandle(T*) -> RcHandle<T>;

template <ref_countable T> RcHandle(std::shared_ptr<T>) -> RcHandle<T>;

IDAFUCKER_NAMESPACE_END