#pragma once
#include <idafucker/CoreDefines.hpp>

IDAFUCKER_NAMESPACE_BEGIN

template <typename From, typename To>
struct IsCastableTo {
  static constexpr auto kValue = requires { static_cast<To>(std::declval<From>()) };
};

template <typename From, typename... To>
struct IsConvertibleToOneOf {
  static constexpr auto kValue = (std::is_convertible<From, To>::value || ...);
};

template <typename From, typename... To>
struct IsCastableToOneOf {
  static constexpr auto kValue = (IsCastableTo<From, To>::kValue || ...);
};

template <typename From, typename... To>
concept convertible_to_one_of = IsConvertibleToOneOf<From, To...>::kValue;

template <typename From, typename... To>
concept castable_to_one_of = IsCastableToOneOf<From, To...>::kValue;

IDAFUCKER_NAMESPACE_END