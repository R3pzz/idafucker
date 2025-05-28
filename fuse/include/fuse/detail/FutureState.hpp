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
    None,
    Value,
    Exception,
  };

public:
  FutureState() noexcept = default;
  ~FutureState() = default;

  FutureState &operator=(const Type &data) {
    if (contained_ != Contained::None) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex_};
    value_ = data;
    contained_ = Contained::Value;
    return *this;
  }

  FutureState &operator=(Type &&data) {
    if (contained_ != Contained::None) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex_};
    value_ = std::move(data);
    contained_ = Contained::Value;
    return *this;
  }

  FutureState &operator=(const std::exception_ptr &eptr) {
    if (contained_ != Contained::None) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex_};
    exception_ = eptr;
    contained_ = Contained::Exception;
    return *this;
  }

  FutureState &operator=(std::exception_ptr &&eptr) {
    if (contained_ != Contained::None) [[unlikely]]
      throw std::runtime_error{"future state has a value"};

    std::lock_guard lock{mutex_};
    exception_ = std::move(eptr);
    contained_ = Contained::Exception;
    return *this;
  }

  [[nodiscard]] constexpr bool hasValue() const noexcept {
    return contained_ == Contained::Value;
  }

  [[nodiscard]] constexpr bool hasException() const noexcept {
    return contained_ == Contained::Exception;
  }

  [[nodiscard]] constexpr bool ready() const noexcept {
    return contained_ != Contained::None;
  }

  [[nodiscard]] const Type &get() const {
    // Wait for some promise to set the value
    std::unique_lock lock{mutex_};
    cv_.wait(lock, [this] { return contained_ != Contained::None; });

    if (contained_ == Contained::Exception)
      std::rethrow_exception(exception_);
    else
      return value_;
  }

  [[nodiscard]] constexpr Type tryGet() const {
    if (contained_ == Contained::Value)
      return value_;
    else if (contained_ == Contained::Exception)
      std::rethrow_exception(exception_);
    return {};
  }

private:
  union {
    Type value_{};
    std::exception_ptr exception_;
  };

  Contained contained_{Contained::None};

  // Multi-threading adapters
  mutable std::condition_variable cv_{};
  mutable std::mutex mutex_{};
};
}  // namespace fuse::detail