#pragma once
#include <expected>    // expected
#include <filesystem>  // path

#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/Underlying.hpp>

IDAFUCKER_NAMESPACE_BEGIN

enum class ResourceErrorCategory {
  ResourceLoaded,
  ResourceMalformed,
  ResourceNotFound,
  ResourceFactoryNotFound,
  ResourceFactoryUnsupportedExtension,
  ResourceDependencyParserNotFound,
};

[[nodiscard]] constexpr auto stringify(ResourceErrorCategory what) noexcept
{
  switch (what) {
    case ResourceErrorCategory::ResourceMalformed:
      return "ResourceMalformed";
    case ResourceErrorCategory::ResourceNotFound:
      return "ResourceNotFound";
    case ResourceErrorCategory::ResourceFactoryNotFound:
      return "ResourceFactoryNotFound";
    case ResourceErrorCategory::ResourceFactoryUnsupportedExtension:
      return "ResourceFactoryUnsupportedExtension";
    case ResourceErrorCategory::ResourceDependencyParserNotFound:
      return "ResourceDependencyParserNotFound";
    default:
      return "<unknown>";
  }
}

class ResourceError final : public std::runtime_error {
 public:
  ResourceError(ResourceErrorCategory category) noexcept
      : std::runtime_error{nullptr}, category_{category}
  {
  }

  ResourceError(ResourceErrorCategory category,
                const std::string& path) noexcept
      : std::runtime_error{nullptr}, category_{category}, path_{path}
  {
  }

  ResourceError(ResourceErrorCategory category,
                const std::filesystem::path& path) noexcept
      : std::runtime_error{nullptr}, category_{category}, path_{path}
  {
  }

  [[nodiscard]] constexpr ResourceErrorCategory category() const noexcept
  {
    return category_;
  }

  [[nodiscard]] constexpr const std::filesystem::path& path() const noexcept
  {
    return path_;
  }

  [[nodiscard]] virtual const char* what() const override
  {
    const auto formatted = std::format("ResourceError '{}' for a '{}' resource",
                                       stringify(category_), path_.string());
    return formatted.c_str();
  }

 private:
  ResourceErrorCategory category_;
  std::filesystem::path path_;
};

template <class T> using ResourceErrorOr = std::expected<T, ResourceError>;

template <typename T, typename... Args>
[[nodiscard]] auto makeResourceResult(Args&&... args) -> ResourceErrorOr<T>
{
  return ResourceErrorOr<T>{std::unexpect, std::forward<Args>(args)...};
}

template <typename T, typename... Args>
[[nodiscard]] auto makeResourceError(Args&&... args) -> ResourceErrorOr<T>
{
  return ResourceErrorOr<T>{std::in_place, std::forward<Args>(args)...};
}

IDAFUCKER_NAMESPACE_END