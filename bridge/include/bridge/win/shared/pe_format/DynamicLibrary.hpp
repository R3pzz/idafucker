#pragma once
#include <bridge/win/pe_format/PortableExecutable.hpp>
#include <bridge/win/pe_format/Symbol.hpp>

namespace bridge::win
{

class DynamicLibrary final : public PortableExecutable {
public:
  constexpr DynamicLibrary() noexcept = default;

  // Looks up a symbol in the export table by its name.
  [[nodiscard]] Result<Symbol> findExportedSymbol(
      const std::string &name) const noexcept;
};

}  // namespace bridge::win