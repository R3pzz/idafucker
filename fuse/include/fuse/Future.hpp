#pragma once
#include <fuse/Config.hpp>
#include <fuse/detail/FutureState.hpp>

namespace fuse
{
template <typename Type> class Future {
public:
  Future(auto &promise) noexcept : container{promise.container} {}

  ~Future() noexcept = default;

  Future(const Future &) noexcept = default;
  Future &operator=(const Future &) noexcept = default;

  Future(Future &&) noexcept = default;
  Future &operator=(Future &&) noexcept = default;

  [[nodiscard]] operator const Type &() const {
    return container->get();
  }

  [[nodiscard]] const Type &get() const {
    return container->get();
  }

  [[nodiscard]] Type tryGet() const {
    return container->tryGet();
  }

  [[nodiscard]] bool hasValue() const noexcept {
    return container->hasValue();
  }

  [[nodiscard]] bool hasException() const noexcept {
    return container->hasException();
  }

  [[nodiscard]] bool ready() const noexcept {
    return container->ready();
  }

private:
  RCHandle<detail::FutureState<Type>> container;
};

template <class Type> class Promise {
public:
  Promise() noexcept : container{new detail::FutureState<Type>{}} {}

  ~Promise() = default;

  Promise(Promise &&) noexcept = default;
  Promise &operator=(Promise &&) noexcept = default;

  Promise &operator=(const Type &value) {
    *container = value;
    return *this;
  }

  Promise &operator=(Type &&value) {
    *container = std::move(value);
    return *this;
  }

  Promise &operator=(const std::exception_ptr &exception) {
    *container = exception;
    return *this;
  }

  Promise &operator=(std::exception_ptr &&exception) {
    *container = std::move(exception);
    return *this;
  }

  void setValue(const Type &value) {
    container = value;
  }

  void setValue(Type &&value) {
    container = std::forward<Type>(value);
  }

  void setException(const std::exception_ptr &exception) {
    container = exception;
  }

  void setException(std::exception_ptr &&exception) {
    container = std::forward<std::exception_ptr>(exception);
  }

  [[nodiscard]] auto future() const noexcept -> Future<Type> {
    return Future<Type>{*this};
  }

private:
  // We share a container for both a promise and a future as this model
  // is designed to keep the retrieved value alive until the future object
  // gets destroyed.
  RCHandle<detail::FutureState<Type>> container;

  template <class> friend class Future;

  IDAFUCKER_NONCOPYABLE(Promise);
};
}  // namespace fuse