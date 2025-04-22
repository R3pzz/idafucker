#pragma once
#include <hyperui/CoreDefines.hpp>
#include <idafucker/resources/ResourceFactory.hpp>

HYPERUI_NAMESPACE_BEGIN

class HtmlFileFactory final : public idafucker::ResourceFactory {
public:
  constexpr HtmlFileFactory() noexcept = default;

  [[nodiscard]] idafucker::Any construct(
      const std::filesystem::path& dataPath) const override;

  [[nodiscard]] const std::type_info& type() const noexcept override;
};

[[nodiscard]] inline auto makeHtmlFileFactory()
    -> std::shared_ptr<HtmlFileFactory>
{
  return std::make_shared<HtmlFileFactory>();
}

HYPERUI_NAMESPACE_END