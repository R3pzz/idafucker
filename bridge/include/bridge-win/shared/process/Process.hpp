#pragma once
#include <bridge-win/shared/common/Status.hpp>
#include <bridge-win/shared/process/Pid.hpp>
#include <fuse/Bits.hpp>

namespace bridge::win::process
{

class Process final {
public:
  constexpr Process() noexcept = default;
  constexpr Process(Pid pid) noexcept : data_{std::move(pid)} {}
  constexpr Process(HANDLE handle) noexcept : data_{std::move(handle)} {}

  [[nodiscard]] StatusOr<Pid> pid() const noexcept;
  [[nodiscard]] StatusOr<bool> hasDebugger() const noexcept;

private:
  union Data {
    static constexpr std::size_t k_isInPIDMode{static_cast<std::size_t>(1u)
                                               << (sizeof(std::size_t) * 8 - 1u)};

    constexpr Data() noexcept : data_{k_isInPIDMode} {}

    constexpr Data(Pid pid) noexcept
        : data_{static_cast<std::size_t>(pid) | k_isInPIDMode} {}

    constexpr Data(HANDLE handle) noexcept : handle_{std::move(handle)} {}

    [[nodiscard]] constexpr Pid pid() const noexcept {
      return data_.contains(k_isInPIDMode) ? static_cast<Pid>(data_ & ~k_isInPIDMode)
                                           : k_badPid;
    }

    [[nodiscard]] constexpr HANDLE handle() const noexcept {
      return !data_.contains(k_isInPIDMode) ? handle_ : NULL;
    }

    [[nodiscard]] constexpr bool isInPIDMode() const noexcept {
      return data_.contains(k_isInPIDMode);
    }

    [[nodiscard]] constexpr bool isInHandleMode() const noexcept {
      return !data_.contains(k_isInPIDMode);
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
      return (data_ & k_isInPIDMode) == 0u;
    }

  private:
    // These members are made private to avoid confusion - use `pid()` and `handle()` to
    // access the corresponding values.
    fuse::Bits<std::size_t> data_{};
    HANDLE handle_;
  } data_;
};

}  // namespace bridge::win::process