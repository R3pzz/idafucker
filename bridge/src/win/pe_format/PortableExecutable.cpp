#include <bridge/win/pe_format/PortableExecutable.hpp>

namespace bridge::win
{

[[nodiscard]] Result<bool> PortableExecutable::isExecutable() const noexcept {
  return getNTHeader().and_then([](NTHeaderRef nt) -> Result<bool> {
    // Executables do not have the `IMAGE_FILE_DLL` flag set.
    // `IMAGE_SUBSYSTEM_NATIVE` is basically intended for system drivers.
    // So we can safely assume that this is an executable file if both
    // of these are not set.
    return (nt->FileHeader.Characteristics & IMAGE_FILE_DLL) == 0u &&
           nt->OptionalHeader.Subsystem != IMAGE_SUBSYSTEM_NATIVE;
  });
}

[[nodiscard]] Result<bool> PortableExecutable::isDynamicLibrary()
    const noexcept {
  return getNTHeader().and_then([](NTHeaderRef nt) -> Result<bool> {
    return (nt->FileHeader.Characteristics & IMAGE_FILE_DLL) != 0u;
  });
}

[[nodiscard]] Result<bool> PortableExecutable::isSystemDriver() const noexcept {
  return getNTHeader().and_then([](NTHeaderRef nt) -> Result<bool> {
    return nt->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE;
  });
}

[[nodiscard]] Result<DOSHeaderRef> PortableExecutable::getDOSHeader()
    const noexcept {
  auto header = reinterpret_cast<DOSHeaderRef>(base_);
  if (header == nullptr || header->e_magic != k_DOSHeaderMagic) {
    return Status::makeInvalidSignature();
  }

  return header;
}

[[nodiscard]] Result<NTHeaderRef> PortableExecutable::getNTHeader()
    const noexcept {
  return getDOSHeader().and_then(
      [this](DOSHeaderRef dos) -> Result<NTHeaderRef> {
        auto header = reinterpret_cast<NTHeaderRef>(base_ + dos->e_lfanew);
        if (header->Signature != k_NTHeaderMagic) {
          return Status::makeInvalidSignature();
        }

        return header;
      });
}

[[nodiscard]] Result<DataDirectoryRef> PortableExecutable::getDataDirectory(
    DataDirectoryType type) const noexcept {
  return getNTHeader().and_then(
      [this, type](NTHeaderRef nt) -> Result<DataDirectoryRef> {
        const auto directory =
            &nt->OptionalHeader.DataDirectory[fuse::underlyingCast(type)];
        if (directory->VirtualAddress == 0 || directory->Size == 0) {
          return Status::makeBadAddress();
        }
        
        return directory;
      });
}

}  // namespace bridge::win