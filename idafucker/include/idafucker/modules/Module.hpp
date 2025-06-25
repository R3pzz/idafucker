#pragma once
#include <string>  // wstring
#include <vector>  // vector

#include <idafucker/Config.hpp>

#include <fuse/Hash.hpp>

//
// The flow:
//   - System scans known module folders for any .dll/.so files;
//   - If the file is found, system loads the file into memory,
//     finds its entry point and the `module_interface` export;
//   - If the export is found, the system caches its directory
//     and proceeds to checking other modules.
//   - When all modules are cached, systems checks the config file
//     to see which modules should be loaded.
//   - For each module, it instantiates its own sandbox(custom
//     allocator, custom memory read/write ops, etc...).
//   - After that, it checks for its dependencies, and loads them.
//     if any of the dependencies is not found, the process aborts.
//   - After loading the dependencies, the main() function is called.
//

namespace idafucker
{

class Module final {
public:
  using Identifier = std::size_t;
  using EntryPoint = int(int, char **);

  constexpr Module(const wchar_t *name) noexcept
      : name_{name} {}

  constexpr Module(std::wstring name) noexcept
      : name_{std::move(name)} {}

  [[nodiscard]] constexpr const std::wstring &name() const noexcept { return name_; }

private:
  std::wstring name_{};  //< Name of the module.
  EntryPoint *entryPoint_{};
};

}  // namespace idafucker