#pragma once
#include <regex>   // regex
#include <vector>  // vector

#include <bridge-win/shared/process/Process.hpp>

namespace bridge::win::process
{

struct ProcessRegistry final {
  // Try to find a process matching its full name against a string passed in `name`. If
  // `openInPlace` is `true`, resulting `Process` will have an open handle to the process.
  // Otherwise, the resulting `Process` will be referenced by its PID.
  [[nodiscard]] static StatusOr<Process> find(std::wstring_view name,
                                              const bool openInPlace = true) noexcept;

  // Try to find a process matching its full name against a regex passed in `expression`.
  // Returns the first process that was found.
  [[nodiscard]] static StatusOr<Process> findFirstMatching(
      const std::wregex &expression) noexcept;
  // Try to find a process matching its full name against a regex passed in `expression`.
  // Returns an `std::vector` of all processes that the regex matched against.
  [[nodiscard]] static StatusOr<std::vector<Process>> findAllMatching(
      const std::wregex &expression) noexcept;
};

}  // namespace bridge::win::process