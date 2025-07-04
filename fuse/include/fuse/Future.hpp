#pragma once
#include <fuse/Class.hpp>
#include <fuse/Config.hpp>
#include <fuse/detail/FutureState.hpp>

namespace fuse
{
template <typename Type>
class Future {
public:
  Future(auto &promise) noexcept : state_{promise.state_} {}

  ~Future() noexcept = default;

  Future(const Future &) noexcept = default;
  Future &operator=(const Future &) noexcept = default;

  Future(Future &&) noexcept = default;
  Future &operator=(Future &&) noexcept = default;

  [[nodiscard]] operator const Type &() const {
    return state_->get();
  }

  [[nodiscard]] const Type &get() const {
    return state_->get();
  }

  [[nodiscard]] Type tryGet() const {
    return state_->tryGet();
  }

  [[nodiscard]] bool hasValue() const noexcept {
    return state_->hasValue();
  }

  [[nodiscard]] bool hasException() const noexcept {
    return state_->hasException();
  }

  [[nodiscard]] bool ready() const noexcept {
    return state_->ready();
  }

private:
  RCHandle<detail::FutureState<Type>> state_;
};

template <typename Type>
class Promise {
public:
  Promise() noexcept : state_{new detail::FutureState<Type>{}} {}

  ~Promise() = default;

  Promise(Promise &&) noexcept = default;
  Promise &operator=(Promise &&) noexcept = default;

  Promise &operator=(const Type &value) {
    *state_ = value;
    return *this;
  }

  Promise &operator=(Type &&value) {
    *state_ = std::move(value);
    return *this;
  }

  Promise &operator=(const std::exception_ptr &exception) {
    *state_ = exception;
    return *this;
  }

  Promise &operator=(std::exception_ptr &&exception) {
    *state_ = std::move(exception);
    return *this;
  }

  void setValue(const Type &value) {
    state_ = value;
  }

  void setValue(Type &&value) {
    state_ = std::forward<Type>(value);
  }

  void setException(const std::exception_ptr &exception) {
    state_ = exception;
  }

  void setException(std::exception_ptr &&exception) {
    state_ = std::forward<std::exception_ptr>(exception);
  }

  [[nodiscard]] auto future() const noexcept -> Future<Type> {
    return Future<Type>{*this};
  }

private:
  // We share a state for both a promise and a future as this model
  // is designed to keep the retrieved value alive until the future object
  // gets destroyed.
  RCHandle<detail::FutureState<Type>> state_;

  template <class>
  friend class Future;

  FUSE_NONCOPYABLE(Promise);
};
}  // namespace fuse