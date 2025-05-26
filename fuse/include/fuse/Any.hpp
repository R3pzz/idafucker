#pragma once
#include <fuse/Config.hpp>
#include <fuse/detail/AnyBase.hpp>

namespace fuse
{
// 32 for an average class size and 8 for a vtable pointer
using Any = detail::AnyBase<40u>;

template <typename T, typename... Args>
[[nodiscard]] Any makeAny(Args&&... args) {
  return Any{T{std::forward<Args>(args)...}};
}
}  // namespace fuse