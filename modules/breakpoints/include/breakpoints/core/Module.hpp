#pragma once
#include <module_api.hpp>

namespace breakpoints::core
{

class BreakpointsModule final : public Module {
public:
  constexpr BreakpointsModule() noexcept = default;

  [[nodiscard]] std::wstring_view name() const override {
    return L"Breakpoints";
  }

  // A function that returns the version of the module.
  [[nodiscard]] std::wstring_view version() const override {
    return L"";
  }
  
  // A function that returns a version of the engine this
  // module was built for.
  [[nodiscard]] std::wstring_view minimalRequiredEngineVersion() const = 0;
};

} // namespace breakpoints