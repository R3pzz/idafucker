#pragma once
#include <fuse/Config.hpp>
#include <fuse/detail/AnyBase.hpp>

namespace fuse
{
// 32 for an average class size and 8 for a vtable pointer
using Any = detail::AnyBase<40u>;

[[nodiscard]] Any makeAny(auto &&object) {
  return Any{std::forward<decltype(object)>(object)};
}

template <typename T>
[[nodiscard]] Any makeInPlaceAny(auto&&... args) {
  return Any{T{std::forward<decltype(args)>(args)...}};
}
}  // namespace fuse