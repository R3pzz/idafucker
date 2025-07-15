#pragma once

#include <string> // wstring

class Module {
public:
  virtual constexpr ~Module() noexcept = default;

  // A function that returns a unicode name of the module.
  // This name will be used everywhere in the UI and for
  // module identification.
  [[nodiscard]] virtual std::wstring_view name() const = 0;

  // A function that returns the version of the module.
  [[nodiscard]] virtual std::wstring_view version() const = 0;
  
  // A function that returns a version of the engine this
  // module was built for.
  [[nodiscard]] virtual std::wstring_view minimalRequiredEngineVersion() const = 0;
};

extern "C" {
  // Returns a `Module` class that resembles the API of the
  // current module.
  export Module *module_interface();
}