#include <hyperui/data/HtmlFile.hpp>
#include <hyperui/data/factories/HtmlFileFactory.hpp>

HYPERUI_NAMESPACE_BEGIN

[[nodiscard]] idafucker::Any HtmlFileFactory::construct(
    const std::istream& stream) const
{
  return idafucker::makeAny<HtmlFile>(const_cast<std::istream&>(stream));
}

[[nodiscard]] const std::type_info& HtmlFileFactory::type() const noexcept
{
  return typeid(HtmlFile);
}

HYPERUI_NAMESPACE_END