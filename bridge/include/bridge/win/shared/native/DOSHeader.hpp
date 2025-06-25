#pragma once
#include <fuse/Platform.hpp>

namespace bridge::win
{

// We define a reference as a const-pointer, since, normally, when applied to
// loaded modules, the headers lie in a read-only memory(unless the image was
// not loaded by the system). We cannot use const references, because then the
// concept of `Result` will not work, since `std::expected` requires that the
// returned object is not a reference.
using DOSHeaderRef = const IMAGE_DOS_HEADER *;

constexpr WORD k_DOSHeaderMagic{0x5A4D};  // MZ

}  // namespace bridge::win