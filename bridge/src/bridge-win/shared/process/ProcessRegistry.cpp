#include <bridge-win/shared/process/ProcessRegistry.hpp>
#include <tlhelp32.h>

namespace bridge::win::process
{

namespace
{

template <typename Fn, typename... Args>
[[nodiscard]] auto visitProcesses(Fn &&function, Args &&...args)
    -> StatusOr<PROCESSENTRY32W> {
  static_assert(
      std::is_same_v<std::invoke_result_t<Fn, const PROCESSENTRY32W &, Args...>, bool>,
      "`function` must return `bool`");
  static_assert(std::is_invocable_v<Fn, const PROCESSENTRY32W &, Args...>,
                "`function` must be invocable with a reference to the process entry");

  // Create the snapshot
  auto snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0u);
  BRIDGE_WIN_CHECK_NULL(snapshot, Status::makeOS());

  // Iterate on the f
  PROCESSENTRY32W entry{.dwSize = sizeof(PROCESSENTRY32W)};
  for (auto success = ::Process32FirstW(snapshot, &entry); success != FALSE;
       success = ::Process32NextW(snapshot, &entry)) {
    if (function(entry, std::forward<Args>(args)...)) {
      return entry;
    }
  }

  return Status::makeNotFound();
}

}  // namespace

[[nodiscard]] StatusOr<Process> ProcessRegistry::find(const std::wstring_view &name,
                                                      const bool openInPlace) noexcept {
  if (name.empty()) [[unlikely]] {
    return Status::makeBadArgument();
  }

  return visitProcesses(
             [&name](const auto &entry) -> bool { return entry.szExeFile == name; })
      .and_then([openInPlace](const auto &entry) -> StatusOr<Process> {
        if (openInPlace) {
          auto handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
          BRIDGE_WIN_CHECK_NULL(handle, Status::makeOS());
          return Process{std::move(handle)};
        } else {
          return Process{entry.th32ProcessID};
        }
      });
}

[[nodiscard]] StatusOr<Process> ProcessRegistry::findFirstMatching(
    const std::wregex &expression) noexcept {
  return visitProcesses([&expression](const auto &entry) -> bool {
           return std::regex_search(entry.szExeFile, expression);
         })
      .and_then([](const auto &entry) -> StatusOr<Process> {
        return Process{::OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID)};
      });
}

[[nodiscard]] StatusOr<std::vector<Process>> ProcessRegistry::findAllMatching(
    const std::wregex &expression) noexcept {
  std::vector<Process> processes{};

  const auto status = visitProcesses(
      [&expression, &processes](const auto &entry) -> bool {
        if (std::regex_search(entry.szExeFile, expression)) {
          processes.emplace_back(
              ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID));
        }

        return false;
      });

  if (!status.has_value() && status.error().code() != Status::Code::NotFound) {
    return status.error();
  }

  return processes;
}

}  // namespace bridge::win::process