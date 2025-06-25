#pragma once
#include <bridge/shared/NativeStructure.hpp>
#include <bridge/win/native/ListEntry.hpp>
#include <bridge/win/native/PEB.hpp>
#include <fuse/Memory.hpp>
#include <fuse/Platform.hpp>

namespace bridge::win
{

template <typename Address> struct TIB : NativeStructure {
  static constexpr auto k_is64Bit = sizeof(Address) == 0x8u;

  static constexpr NativeField<Address> k_exceptionList{0x0u};
  static constexpr NativeField<Address> k_stackBase{k_is64Bit ? 0x8u : 0x4u};
  static constexpr NativeField<Address> k_stackLimit{k_is64Bit ? 0x10u : 0x8u};
  static constexpr NativeField<Address> k_arbitraryUserPointer{
      k_is64Bit ? 0x28u : 0x1cu};
  static constexpr NativeField<Address> k_self{k_is64Bit ? 0x30u : 0x20u};
};

template <typename Address> struct ClientId : NativeStructure {
  static constexpr auto k_is64Bit = sizeof(Address) == 0x8u;

  static constexpr NativeField<Address> k_uniqueProcess{0x0u};
  static constexpr NativeField<Address> k_uniqueThread{k_is64Bit ? 0x8u : 0x4u};
};

template <typename Address> struct TEB : NativeStructure {
  static constexpr auto k_is64Bit = sizeof(Address) == 0x8u;

  static constexpr NativeField<TIB<Address>> k_TIB{0x0u};
  static constexpr NativeField<ClientId<Address>> k_clientId{k_is64Bit ? 0x40u
                                                                       : 0x20u};
};

}  // namespace bridge::win