#pragma once
#include <hyperui/CoreDefines.hpp>
#include <hyperui/data/HtmlFile.hpp>
#include <idafucker/resources/ResourceFactory.hpp>

HYPERUI_NAMESPACE_BEGIN

class HtmlFileFactory final : public idafucker::ResourceFactory {
 public:
  constexpr HtmlFileFactory() noexcept = default;

  [[nodiscard]] idafucker::Any construct(
      const std::istream& stream) const override
  {
    return idafucker::makeAny<HtmlFile>(const_cast<std::istream&>(stream));
  }

  [[nodiscard]] const std::type_info& type() const noexcept override
  {
    return typeid(HtmlFile);
  }
};

[[nodiscard]] auto makeHtmlFileFactory() -> std::shared_ptr<HtmlFileFactory>
{
  return std::make_shared<HtmlFileFactory>();
}

HYPERUI_NAMESPACE_END