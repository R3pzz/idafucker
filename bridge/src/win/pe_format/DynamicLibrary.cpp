#include <bridge/win/pe_format/DynamicLibrary.hpp>

namespace bridge::win
{

[[nodiscard]] Result<Symbol> DynamicLibrary::findExportedSymbol(
    const std::string &name) const noexcept {
  if (name.empty()) {
    return Status::makeInvalidArgument();
  }

  const auto exportDirectory =
      getDataDirectory(DataDirectoryType::Export)
          .and_then([this](DataDirectoryRef directory)
                        -> Result<const IMAGE_EXPORT_DIRECTORY *> {
            return reinterpret_cast<const IMAGE_EXPORT_DIRECTORY *>(
                base_ + directory->VirtualAddress);
          });
  if (!exportDirectory) {
    return exportDirectory.error();
  }

  const auto names = reinterpret_cast<std::uint32_t *>(
      base_ + exportDirectory.value()->AddressOfNames);
  const auto ordinals = reinterpret_cast<std::uint32_t *>(
      base_ + exportDirectory.value()->AddressOfNameOrdinals);
  const auto functions = reinterpret_cast<std::uint32_t *>(
      base_ + exportDirectory.value()->AddressOfFunctions);

  for (size_t i = 0; i < exportDirectory.value()->NumberOfNames; ++i) {
    const auto symbolName = reinterpret_cast<const char *>(base_ + names[i]);
    if (name == symbolName) {
      //return Symbol{symbolName, base_ + functions[ordinals[i]]};
      return Symbol{};
    }
  }

  return Status::makeSymbolNotFound();
}

}  // namespace bridge::win