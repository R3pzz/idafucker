#pragma once
#include <bridge/shared/Status.hpp>

namespace bridge
{

template <typename Type> using Result = std::expected<Type, Status>;

}  // namespace bridge