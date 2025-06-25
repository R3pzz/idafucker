#include "StatusFormatter.hpp"

namespace bridge
{

[[nodiscard]] std::string StatusFormatter::format(const Status &status) {
  std::string formatted{};
  formatted += std::format("Code: `{}` ({})\n", stringify(status.code()),
                           fuse::underlyingCast(status.code()));
  formatted += format(status.trace_);

  if (!status.payloads_.empty()) {
    formatted += "Additional messages:\n";
  }
  for (auto &&payload : status.payloads_) {
    formatted += format(payload);
  }

  return formatted;
}

[[nodiscard]] std::string StatusFormatter::format(
    const std::stacktrace &trace) {
  std::string formatted{"Trace:\n"};
  for (std::size_t i{}; i < trace.size(); ++i) {
    const auto &frame = trace.at(i);
    formatted += std::format("  {}> {}({}): {}\n", i, frame.source_file(),
                             frame.source_line(), frame.description());
  }

  return formatted;
}

[[nodiscard]] std::string StatusFormatter::format(const std::string &string) {
  return std::format("  - `{}`;\n", string);
}

}  // namespace bridge