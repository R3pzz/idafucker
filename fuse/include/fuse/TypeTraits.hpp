#pragma once
#include <fuse/Config.hpp>

namespace fuse
{
//
// Convertibility/cast checks
//

template <typename From, typename To>
constexpr auto is_castable_to_v =
    requires(From from) { static_cast<To>(from); };

template <typename From, typename... To>
constexpr auto is_convertible_to_one_of_v =
    (std::is_convertible_v<From, To> || ...);

template <typename From, typename... To>
constexpr auto is_castable_to_one_of_v = (is_castable_to_v<From, To> || ...);

template <typename From, typename To>
struct is_castable_to : std::bool_constant<is_castable_to_v<From, To>> {};

template <typename From, typename... To>
struct is_convertible_to_one_of
    : std::bool_constant<is_convertible_to_one_of_v<From, To...>> {};

template <typename From, typename... To>
struct is_castable_to_one_of
    : std::bool_constant<is_castable_to_one_of_v<From, To...>> {};

template <typename From, typename To>
concept castable_to = is_castable_to_v<From, To>;

template <typename From, typename... To>
concept convertible_to_one_of = is_convertible_to_one_of_v<From, To...>;

template <typename From, typename... To>
concept castable_to_one_of = is_castable_to_one_of_v<From, To...>;

//
// Type comparison
//

template <typename T, typename... Types>
constexpr auto is_any_of_v = std::disjunction_v<std::is_same<T, Types>...>;

template <typename Derived, typename... Bases>
constexpr auto is_any_base_of_v =
    std::disjunction_v<std::is_base_of<Bases, Derived>...>;

template <typename T>
concept scalar = std::is_scalar<T>::value;

// Function traits

template <typename> struct function_traits {};

template <typename Return, typename... Args>
struct function_traits<Return(Args...)> {
  using return_type = Return;
  using args_type =
      std::conditional_t<sizeof...(Args) == 0u, void, std::tuple<Args...>>;

  template <std::size_t I> using arg_at = std::tuple_element_t<I, args_type>;

  static constexpr bool has_args = sizeof...(Args) != 0u;
};
}  // namespace fuse