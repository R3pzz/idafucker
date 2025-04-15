#pragma once
#include <memory>  // shared_ptr

#include <idafucker/CoreDefines.hpp>
#include <idafucker/exceptions/Exception.hpp>

//
// An Intrusive Handle is a auto-ref-counted handle to an object,
// which should provide such members:
//
//  * onReferenceAdded() -> increments object's reference count;
//  * onReferenceRemoved() -> decrements object's reference count.
//

IDAFUCKER_NAMESPACE_BEGIN

// clang-format off
template <typename T>
concept ref_countable = requires(T t) {
  {t.onReferenceAdded()};
  {t.onReferenceRemoved()};
};

// clang-format on

template <ref_countable T> class IntrusiveHandle {
 public:
  constexpr IntrusiveHandle() noexcept = default;

  // Construct from a raw pointer
  explicit IntrusiveHandle(T* ref) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()))
      : handle_{ref}
  {
    if (handle_ != nullptr) {
      handle_->onReferenceAdded();
    }
  }

  // Construct from another handle
  explicit IntrusiveHandle(const IntrusiveHandle& other) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()))
      : handle_{other.handle_}
  {
    if (handle_ != nullptr) {
      handle_->onReferenceAdded();
    }
  }

  // Construct from a nullptr
  explicit constexpr IntrusiveHandle(std::nullptr_t) noexcept : handle_{nullptr}
  {
  }

  // Construct from shared_ptr
  explicit constexpr IntrusiveHandle(const std::shared_ptr<T>& ref) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()))
      : handle_{ref.get()}
  {
    if (handle_ != nullptr) {
      handle_->onReferenceAdded();
    }
  }

  ~IntrusiveHandle() noexcept(noexcept(std::declval<T>().onReferenceRemoved()))
  {
    handle_->onReferenceRemoved();
  }

  [[nodiscard]] constexpr bool operator==(std::nullptr_t) const noexcept
  {
    return handle_ == nullptr;
  }

  // Assign from a raw pointer
  auto& operator=(T* ref) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()) && noexcept(
          std::declval<T>().onReferenceRemoved()))
  {
    if (handle_ != nullptr) {
      handle_->onReferenceRemoved();
    }

    handle_ = ref;
    if (handle_ != nullptr) {
      handle_->onReferenceAdded();
    }

    return *this;
  }

  // Copy-assign from another handle
  auto& operator=(const IntrusiveHandle& rhs) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()) && noexcept(
          std::declval<T>().onReferenceRemoved()))
  {
    if (handle_ != nullptr) {
      handle_->onReferenceRemoved();
    }

    handle_ = rhs.handle_;
    if (handle_ != nullptr) {
      handle_->onReferenceAdded();
    }

    return *this;
  }

  // Copy-assign from another handle
  auto& operator=(const std::shared_ptr<T>& rhs) noexcept(
      noexcept(std::declval<T>().onReferenceAdded()) && noexcept(
          std::declval<T>().onReferenceRemoved()))
  {
    if (handle_ != nullptr) {
      handle_->onReferenceRemoved();
    }

    handle_ = rhs.get();
    if (handle_ != nullptr) {
      handle_->onReferenceAdded();
    }

    return *this;
  }

  // Assign from a nullptr
  auto& operator=(std::nullptr_t) noexcept(
      noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (handle_ != nullptr) {
      handle_->onReferenceRemoved();
    }

    handle_ = nullptr;
    return *this;
  }

  [[nodiscard]] constexpr auto operator->() const noexcept -> T*
  {
    return handle_;
  }

  // Dereferencing
  [[nodiscard]] constexpr T& operator*()
  {
    if (handle_ == nullptr) {
      throw Exception{"Bad intrusive handle access"};
    }

    return *handle_;
  }

  [[nodiscard]] constexpr const T& operator*() const
  {
    if (handle_ == nullptr) {
      throw Exception{"Bad intrusive handle access"};
    }

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
  void reset() noexcept(noexcept(std::declval<T>().onReferenceRemoved()))
  {
    if (handle_ != nullptr) {
      handle_->onReferenceRemoved();
    }
    handle_ = nullptr;
  }

 private:
  T* handle_{};
};

template <ref_countable T> IntrusiveHandle(T*) -> IntrusiveHandle<T>;

template <ref_countable T>
IntrusiveHandle(std::shared_ptr<T>) -> IntrusiveHandle<T>;

IDAFUCKER_NAMESPACE_END