#pragma once
#include <bridge/shared/Result.hpp>
#include <bridge/win/shared/native/DOSHeader.hpp>
#include <bridge/win/shared/native/DataDirectory.hpp>
#include <bridge/win/shared/native/NTHeader.hpp>
#include <fuse/Enum.hpp>

namespace bridge::win
{

// Represents a generic PE-format file that can be a dynamic library (.dll), an
// executable file (.exe), a system driver (.sys), etc...
class PortableExecutable {
public:
  constexpr PortableExecutable() noexcept = default;

  constexpr PortableExecutable(std::uintptr_t base) noexcept : base_{base} {}

  [[nodiscard]] Result<bool> isExecutable() const noexcept;
  [[nodiscard]] Result<bool> isDynamicLibrary() const noexcept;
  [[nodiscard]] Result<bool> isSystemDriver() const noexcept;

  // Gets the DOS header of a PE file. This may fail if the magic number of the
  // header is wrong.
  [[nodiscard]] Result<DOSHeaderRef> getDOSHeader() const noexcept;

  // Gets the NT header of a PE file. Fails only if `getDOSHeader` also fails.
  [[nodiscard]] Result<NTHeaderRef> getNTHeader() const noexcept;

  // Gets a specified data directory for a PE file.
  [[nodiscard]] Result<DataDirectoryRef> getDataDirectory(
      DataDirectoryType type) const noexcept;

protected:
  std::uintptr_t base_{};
};

}  // namespace bridge::win