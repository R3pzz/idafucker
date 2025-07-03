#pragma once
#include <filesystem> // path

#include <introspect/Config.hpp>

namespace introspect
{

class ModuleVerifier final {
public:
  static constexpr auto k_moduleInterface{L"module_interface"};
};

} // namespace introspect