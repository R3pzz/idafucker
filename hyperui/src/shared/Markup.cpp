#include <fstream>   // wifstream
#include <iterator>  // istreambuf_iterator

#include <hyperui/Markup.hpp>

namespace hyperui
{

[[nodiscard]] fuse::Any Markup::Factory::construct(
    const std::filesystem::path& dataPath) const {
  std::wifstream stream{dataPath};
  if (!stream.is_open()) [[unlikely]]
    throw std::runtime_error{"bad data path"};

  return fuse::makeAny<Markup>(std::wstring{std::istreambuf_iterator<wchar_t>{stream},
                                            std::istreambuf_iterator<wchar_t>{}});
}

[[nodiscard]] const std::type_info& Markup::Factory::type() const noexcept {
  return typeid(Markup);
}

[[nodiscard]] introspect::ResourceFactory::Ref Markup::Factory::make() {
  return std::make_shared<Factory>();
}

}  // namespace hyperui