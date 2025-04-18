#pragma once
#include <atomic>              // atomic_size_t
#include <condition_variable>  // condition_variable
#include <mutex>               // mutex

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/RefCount.hpp>
#include <idafucker/exceptions/Exception.hpp>

using namespace std::chrono_literals;

IDAFUCKER_NAMESPACE_BEGIN

namespace detail
{
template <typename Type>
class ValueContainer : public RefCountable<ConcurrentModel::Atomic> {
private:
  enum class Contained {
    None,
    Value,
    Exception,
  };

public:
  ValueContainer() noexcept = default;
  ~ValueContainer() = default;

  ValueContainer &operator=(const Type &data)
  {
    expectContainedType(Contained::None);

    std::lock_guard lock{mutex};
    value = data;
    contained = Contained::Value;
    return *this;
  }

  ValueContainer &operator=(Type &&data)
  {
    // You can only assign once
    expectContainedType(Contained::None);

    std::lock_guard lock{mutex};
    value = std::move(data);
    contained = Contained::Value;
    return *this;
  }

  ValueContainer &operator=(const std::exception_ptr &eptr)
  {
    expectContainedType(Contained::None);

    std::lock_guard lock{mutex};
    exception = eptr;
    contained = Contained::Exception;
    return *this;
  }

  ValueContainer &operator=(std::exception_ptr &&eptr)
  {
    expectContainedType(Contained::None);

    std::lock_guard lock{mutex};
    exception = std::move(eptr);
    contained = Contained::Exception;
    return *this;
  }

  [[nodiscard]] constexpr bool hasValue() const noexcept
  {
    return contained == Contained::Value;
  }

  [[nodiscard]] constexpr bool hasException() const noexcept
  {
    return contained == Contained::Exception;
  }

  [[nodiscard]] constexpr bool ready() const noexcept
  {
    return contained != Contained::None;
  }

  // Add monadics through Monadic<> here

  [[nodiscard]] const Type &get() const
  {
    // Wait for some promise to set the value
    std::unique_lock lock{mutex};
    cv.wait(lock, [this] { return contained != Contained::None; });

    if (contained == Contained::Exception)
      std::rethrow_exception(exception);
    else
      return value;
  }

  [[nodiscard]] constexpr Type tryGet() const
  {
    if (contained == Contained::Value)
      return value;
    else if (contained == Contained::Exception)
      std::rethrow_exception(exception);
    return {};
  }

private:
  constexpr void expectContainedType(Contained type) const
  {
    if (contained != type) [[unlikely]]
      throw Exception{"ValueContainer contains a different type"};
  }

  union {
    Type value{};
    std::exception_ptr exception;
  };

  Contained contained{Contained::None};

  // Multi-threading adapters
  mutable std::condition_variable cv{};
  mutable std::mutex mutex{};
};
}  // namespace detail

IDAFUCKER_NAMESPACE_END

#include <future>