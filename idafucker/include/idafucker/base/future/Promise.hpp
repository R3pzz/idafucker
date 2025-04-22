#pragma once
#include <idafucker/CoreDefines.hpp>

#include "Future.hpp"

IDAFUCKER_NAMESPACE_BEGIN

template <class Type> class Promise {
public:
  Promise() noexcept : container{new detail::ValueContainer<Type>{}} {}

  ~Promise() = default;

  Promise(Promise &&) noexcept = default;
  Promise &operator=(Promise &&) noexcept = default;

  Promise &operator=(const Type &value)
  {
    *container = value;
    return *this;
  }

  Promise &operator=(Type &&value)
  {
    *container = std::move(value);
    return *this;
  }

  Promise &operator=(const std::exception_ptr &exception)
  {
    *container = exception;
    return *this;
  }

  Promise &operator=(std::exception_ptr &&exception)
  {
    *container = std::move(exception);
    return *this;
  }

  void setValue(const Type &value)
  {
    container = value;
  }

  void setValue(Type &&value)
  {
    container = std::forward<Type>(value);
  }

  void setException(const std::exception_ptr &exception)
  {
    container = exception;
  }

  void setException(std::exception_ptr &&exception)
  {
    container = std::forward<std::exception_ptr>(exception);
  }

  [[nodiscard]] auto future() const noexcept -> Future<Type>
  {
    return Future<Type>{*this};
  }

private:
  // We share a container for both a promise and a future as this model
  // is designed to keep the retrieved value alive until the future object
  // gets destroyed.
  RcHandle<detail::ValueContainer<Type>> container;

  template <class> friend class Future;

  IDAFUCKER_NONCOPYABLE(Promise);
};

IDAFUCKER_NAMESPACE_END