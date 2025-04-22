#include <hyperui/data/HtmlFile.hpp>
#include <hyperui/data/factories/HtmlFileFactory.hpp>
#include <idafucker/exceptions/Exception.hpp>

#include <fstream> // ifstream

HYPERUI_NAMESPACE_BEGIN

[[nodiscard]] idafucker::Any HtmlFileFactory::construct(
    const std::filesystem::path& dataPath) const
{
  std::ifstream stream{dataPath};
  if (!stream.is_open())
    throw idafucker::Exception{"Cannot open `"} << dataPath.string() << "`";

  return idafucker::makeAny<HtmlFile>(stream);
}

[[nodiscard]] const std::type_info& HtmlFileFactory::type() const noexcept
{
  return typeid(HtmlFile);
}

HYPERUI_NAMESPACE_END