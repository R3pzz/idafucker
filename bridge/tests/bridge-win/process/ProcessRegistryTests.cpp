#include <bridge.hpp>

// For testing
#include <catch.hpp>

namespace
{

static constexpr auto k_tragetProcessName{L"explorer.exe"};

} // namespace

TEST_CASE("Full-name-match process discovery") {
  // Try to find the Windows Explorer process. Explorer is always ran on all modern
  // Windows machines(unless terminated by the user), so this case should never fail.
  auto process = bridge::process::ProcessRegistry::find(k_tragetProcessName, false);
  REQUIRE(process.has_value());
  REQUIRE_NOTHROW(process->pid());
}