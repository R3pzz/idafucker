#pragma once
#include <bridge/shared/NativeField.hpp>
#include <bridge/shared/NativeStructure.hpp>
#include <bridge/shared/Result.hpp>
#include <bridge/shared/Status.hpp>

#include <fuse/Config.hpp>
#if FUSE_PLATFORM_WIN
  #include <bridge/win/native/DataDirectory.hpp>
  #include <bridge/win/native/DOSHeader.hpp>
  #include <bridge/win/native/ListEntry.hpp>
  #include <bridge/win/native/NTHeader.hpp>
  #include <bridge/win/native/PEB.hpp>
  #include <bridge/win/native/TEB.hpp>
  #include <bridge/win/native/UnicodeString.hpp>
  #include <bridge/win/pe_format/DynamicLibrary.hpp>
  #include <bridge/win/pe_format/PortableExecutable.hpp>
  #include <bridge/win/pe_format/Symbol.hpp>
  #include <bridge/win/process/Process.hpp>
  #include <bridge/win/process/ProcessEnvironment.hpp>
  #include <bridge/win/process/ProcessRegistry.hpp>
#elif FUSE_PLATFORM_LINUX
  #error "Not supported yet..."
#else
  #error "Unsupported platform."
#endif

namespace bridge
{

#if FUSE_PLATFORM_WIN
using namespace ::bridge::win;
#elif FUSE_PLATFORM_LINUX
using namespace ::bridge::linux;
#endif

} // namespace bridge