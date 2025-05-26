#pragma once
#include <atomic>              // atomic_size_t
#include <condition_variable>  // condition_variable
#include <mutex>               // mutex

#include <fuse/Config.hpp>
#include <fuse/RefCount.hpp>

namespace fuse::detail
{
template <typename Type> class FutureState : public RefCountable<> {
private:
  enum class Contained {
    kNone,
    kValue,
    kException,
  };

public:
  FutureState() noexcept = default;
  ~FutureState() = default;

  FutureState &operator=(const Type &data) {
    if (contained != Contained::kNone) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex};
    value = data;
    contained = Contained::kValue;
    return *this;
  }

  FutureState &operator=(Type &&data) {
    if (contained != Contained::kNone) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex};
    value = std::move(data);
    contained = Contained::kValue;
    return *this;
  }

  FutureState &operator=(const std::exception_ptr &eptr) {
    if (contained != Contained::kNone) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex};
    exception = eptr;
    contained = Contained::kException;
    return *this;
  }

  FutureState &operator=(std::exception_ptr &&eptr) {
    if (contained != Contained::kNone) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex};
    exception = std::move(eptr);
    contained = Contained::kException;
    return *this;
  }

  [[nodiscard]] constexpr bool hasValue() const noexcept {
    return contained == Contained::kValue;
  }

  [[nodiscard]] constexpr bool hasException() const noexcept {
    return contained == Contained::kException;
  }

  [[nodiscard]] constexpr bool ready() const noexcept {
    return contained != Contained::kNone;
  }

  [[nodiscard]] const Type &get() const {
    // Wait for some promise to set the value
    std::unique_lock lock{mutex};
    cv.wait(lock, [this] { return contained != Contained::kNone; });

    if (contained == Contained::kException)
      std::rethrow_exception(exception);
    else
      return value;
  }

  [[nodiscard]] constexpr Type tryGet() const {
    if (contained == Contained::kValue)
      return value;
    else if (contained == Contained::kException)
      std::rethrow_exception(exception);
    return {};
  }

private:
  union {
    Type value{};
    std::exception_ptr exception;
  };

  Contained contained{Contained::kNone};

  // Multi-threading adapters
  mutable std::condition_variable cv{};
  mutable std::mutex mutex{};
};
}  // namespace fuse::detail