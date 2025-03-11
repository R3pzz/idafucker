#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/meta/RefCountable.hpp>

//
// An Intrusive Handle is a auto-ref-counted handle to an object,
// which should provide such members:
//
//  * onReferenceAdded() -> increments object's reference count;
//  * onReferenceRemoved() -> decrements object's reference count.
//

IDAFUCKER_NAMESPACE_BEGIN

class BadIntrusiveHandleAccess : public std::runtime_error {
public:
  BadIntrusiveHandleAccess() : std::runtime_error{"Bad intrusive handle access"} {}
};

template <ref_countable T>
class IntrusiveHandle {
public:
  constexpr IntrusiveHandle() noexcept = default;

  // Construct from a raw pointer
  explicit IntrusiveHandle(T *ref) noexcept(noexcept(std::declval<T>().onReferenceAdded())) : _handle{ref}
  {
    if (_handle != nullptr)
      _handle->onReferenceAdded();
  }

  // Construct from another handle
  explicit IntrusiveHandle(const IntrusiveHandle &other) noexcept(noexcept(std::declval<T>().onReferenceAdded()))
      : _handle{other._handle}
  {
    if (_handle != nullptr)
      _handle->onReferenceAdded();
  }

  // Construct from a nullptr
  explicit constexpr IntrusiveHandle(std::nullptr_t) noexcept : _handle{nullptr} {}

  // Construct from shared_ptr
  explicit constexpr IntrusiveHandle(const std::shared_ptr<T> &ref) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()))
      : _handle{ref.get()}
  {
    if (_handle != nullptr)
      _handle->onReferenceAdded();
  }

  ~IntrusiveHandle() noexcept(noexcept(std::declval<T>().onReferenceRemoved())) { _handle->onReferenceRemoved(); }

public:
  // Assign from a raw pointer
  auto &operator=(T *ref) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()) && noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (_handle != nullptr)
      _handle->onReferenceRemoved();

    _handle = ref;
    if (_handle != nullptr)
      _handle->onReferenceAdded();

    return *this;
  }
  
  // Copy-assign from another handle
  auto &operator=(const IntrusiveHandle &rhs) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()) && noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (_handle != nullptr)
      _handle->onReferenceRemoved();

    _handle = rhs._handle;
    if (_handle != nullptr)
      _handle->onReferenceAdded();

    return *this;
  }

  // Copy-assign from another handle
  auto &operator=(const std::shared_ptr<T> &rhs) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()) && noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (_handle != nullptr)
      _handle->onReferenceRemoved();
     
    _handle = rhs.get();
    if (_handle != nullptr)
      _handle->onReferenceAdded();

    return *this;
  }

  // Assign from a nullptr
  auto &operator=(std::nullptr_t) noexcept(noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (_handle != nullptr)
      _handle->onReferenceRemoved();

    _handle = nullptr;
    return *this;
  }

  [[nodiscard]] constexpr T *operator->() const noexcept { return _handle; }

  // Dereferencing
  [[nodiscard]] constexpr T &operator*()
  {
    if (_handle == nullptr)
      throw BadIntrusiveHandleAccess{};

    return *_handle;
  }

  [[nodiscard]] constexpr const T &operator*() const
  {
    if (_handle == nullptr)
      throw BadIntrusiveHandleAccess{};

    return *_handle;
  }

public:
  // Raw pointer access
  [[nodiscard]] constexpr T *raw() noexcept { return _handle; }
  [[nodiscard]] constexpr const T *raw() const noexcept { return _handle; }

  // State managing
  void reset() noexcept(noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (_handle != nullptr)
      _handle->onReferenceRemoved();

    _handle = nullptr;
  }

private:
  T *_handle{};
};

IDAFUCKER_NAMESPACE_END