#pragma once
#include <filesystem> // path

#include <idafucker/Config.hpp>

namespace idafucker
{

class ModuleVerifier final {
public:
  static constexpr auto k_moduleInterface{L"module_interface"};
};

} // namespace idafucker