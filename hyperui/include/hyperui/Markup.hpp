#pragma once
#include <hyperui/Config.hpp>
#include <introspect/resources/Resource.hpp>
#include <introspect/resources/ResourceFactory.hpp>

namespace hyperui
{
class Markup {
public:
  class Factory final : public introspect::ResourceFactory {
  public:
    constexpr Factory() noexcept = default;

    [[nodiscard]] fuse::Any construct(
        const std::filesystem::path& dataPath) const override;
    [[nodiscard]] const std::type_info& type() const noexcept override;

    [[nodiscard]] static introspect::ResourceFactory::Ref make();
  };

  constexpr Markup() noexcept = default;
  constexpr Markup(std::wstring buffer) noexcept : buffer_{std::move(buffer)} {}

  [[nodiscard]] constexpr const std::wstring& buffer() const noexcept {
    return buffer_;
  }

private:
  std::wstring buffer_{};
};
}  // namespace hyperui