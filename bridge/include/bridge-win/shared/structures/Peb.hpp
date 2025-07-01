#pragma once
#include <bridge-win/shared/structures/detail/Field.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win::structures
{

struct Peb final {
  static constexpr detail::Field<BOOLEAN, 0x2u> k_beingDebugged{};
};

}  // namespace bridge::win::structures