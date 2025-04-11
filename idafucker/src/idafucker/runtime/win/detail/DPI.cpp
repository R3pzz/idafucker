#include "DPI.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace win::detail {
[[nodiscard]] HRESULT WINAPI SetProcessDpiAwareness(ProcessDpiAwareness level)
{
  using Type = HRESULT(WINAPI)(ProcessDpiAwareness);

  const auto library = ::LoadLibrary(WINDOWS_STRING("shcore.dll"));
  if (library == NULL) {
    return E_FAIL;
  }

  const auto function = reinterpret_cast<Type *>(
      ::GetProcAddress(library, "SetProcessDpiAwareness"));
  if (function == nullptr) {
    return E_FAIL;
  }

  return function(level);
}
}  // namespace win::detail

IDAFUCKER_NAMESPACE_END