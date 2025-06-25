#include <bridge/shared/Status.hpp>
#include "StatusFormatter.hpp"

namespace bridge
{

[[nodiscard]] std::string Status::string() const {
  return StatusFormatter::format(*this);
}

} // namespace bridge