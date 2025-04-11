#pragma once
#include <idafucker/CoreDefines.hpp>

#include "Character.hpp"

IDAFUCKER_NAMESPACE_BEGIN

namespace win::detail {
enum class ProcessDpiAwareness : DWORD {
  Unaware = 0,
  SystemDPIAware = 1,
  PerMonitorDPIAware = 2,
};

[[nodiscard]] HRESULT WINAPI SetProcessDpiAwareness(ProcessDpiAwareness level);
}  // namespace win::detail

IDAFUCKER_NAMESPACE_END