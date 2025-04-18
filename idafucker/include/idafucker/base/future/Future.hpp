#pragma once
#include <idafucker/CoreDefines.hpp>

#include "ValueContainer.hpp"

IDAFUCKER_NAMESPACE_BEGIN

template <typename Type> class Future {
public:
  // Binds the future to a promise
  Future(auto &&promise) noexcept : container{promise.container} {}

  ~Future() noexcept = default;

  Future(const Future &) noexcept = default;
  Future& operator=(const Future &) noexcept = default;

  Future(Future &&) noexcept = default;
  Future &operator=(Future &&) noexcept = default;

  [[nodiscard]] const Type &get() const
  {
    return container->get();
  }

  [[nodiscard]] Type tryGet() const
  {
    return container->tryGet();
  }

  [[nodiscard]] bool hasValue() const noexcept
  {
    return container->hasValue();
  }

  [[nodiscard]] bool hasException() const noexcept
  {
    return container->hasException();
  }
  
  [[nodiscard]] bool ready() const noexcept
  {
    return container->ready();
  }

private:
  RcHandle<detail::ValueContainer<Type>> container;
};

IDAFUCKER_NAMESPACE_END