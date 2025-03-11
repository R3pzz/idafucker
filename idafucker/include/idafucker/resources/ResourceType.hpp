#pragma once
#include <idafucker/CoreDefines.hpp>
#include <idafucker/base/templates/TypeFamily.hpp>

IDAFUCKER_NAMESPACE_BEGIN

// A unique resource type identifier.
//
// The benefit of this class over a standard enum of types is that
// It allows to decouple actual resource type definition from the
// Layer 1. This lets the users to create custom resources without
// modifying Layer 1 code.
class ResourceType {
public:
  constexpr ResourceType() noexcept = default;

#if defined(IDAFUCKER_DEBUG)
  constexpr ResourceType(TypeFamilyId id, const std::string &typeName) noexcept : _id{id}, _typeName{typeName} {}
#else
  constexpr ResourceType(TypeFamilyId id) noexcept : _id{id} {}
#endif // defined(IDAFUCKER_DEBUG)

  [[nodiscard]] constexpr auto operator!=(const ResourceType &rhs) const noexcept { return _id != rhs._id; }
  [[nodiscard]] constexpr auto operator!=(const TypeFamilyId &rhs) const noexcept { return _id != rhs; }

  [[nodiscard]] constexpr auto matches(const ResourceType &other) const noexcept { return _id == other._id; }

#if defined(IDAFUCKER_DEBUG)
  [[nodiscard]] constexpr const auto &typeName() const noexcept { return _typeName; }
#endif // defined(IDAFUCKER_DEBUG)

private:
  // Stores a unique type identifier.
  TypeFamilyId _id{};

#if defined(IDAFUCKER_DEBUG)
  // RTTI support
  std::string _typeName{};
#endif // defined(IDAFUCKER_DEBUG)
};

#if defined(IDAFUCKER_DEBUG)
  #define IDAFUCKER_IMPLEMENT_RESOURCE_TYPE()                                                   \
    [[nodiscard]] virtual ::idafucker::ResourceType type() const noexcept override              \
    {                                                                                           \
      using NonCVQualified = std::remove_cv<decltype(*this)>::type;                             \
      return {::idafucker::TypeFamily<NonCVQualified>::value(), typeid(NonCVQualified).name()}; \
    }
#else
  #define IDAFUCKER_IMPLEMENT_RESOURCE_TYPE()                                      \
    [[nodiscard]] virtual ::idafucker::ResourceType type() const noexcept override \
    {                                                                              \
      using NonCVQualified = std::remove_cv<decltype(*this)>::type;                \
      return {::idafucker::TypeFamily<NonCVQualified>::value()};                   \
    }
#endif // defined(IDAFUCKER_DEBUG)

IDAFUCKER_NAMESPACE_END