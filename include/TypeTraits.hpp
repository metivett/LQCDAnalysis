/*
 * TypeTraits.hpp
 *
 *  Created on: Dec 11, 2013
 *      Author: Thibaut Metivet
 */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

namespace LQCDA
{

namespace internal
{
struct dummy_type {};
}

// template<class T, class U> struct and_;
// template<class T, class U, T t, U u>
// struct and_<std::integral_constant<T,t>, std::integral_constant<U,u>>: std::integral_constant<bool, t && u> {};
template<bool... Ts>
struct and_;
template<bool... Ts>
struct and_<true, Ts...>: and_<Ts...> {};
template<bool... Ts>
struct and_<false, Ts...>: std::false_type {};
template<>
struct and_<>: std::true_type {};

// template<class T, class U> struct or_;
// template<class T, class U, T t, U u>
// struct or_<std::integral_constant<T, t>, std::integral_constant<U, u>>: std::integral_constant < bool, t || u > {};
template<bool... Ts>
struct or_;
template<bool... Ts>
struct or_<true, Ts...>: std::true_type {};
template<bool... Ts>
struct or_<false, Ts...>: or_<Ts...> {};
template<>
struct or_<>: std::false_type {};

template<bool T> struct not_: std::integral_constant < bool, !T > {};
// template<class T> struct not_;
// template<class T, T t>
// struct not_<std::integral_constant<T, t>>: std::integral_constant < bool, !t > {};

template<bool Condition, typename T, typename F> struct if_;
template<typename T, typename F>
struct if_<true, T, F>
{
    typedef T result;
};
template<typename T, typename F>
struct if_<false, T, F>
{
    typedef F result;
};


template<typename ... A>
struct are_voids;
template<typename T, typename ... A>
struct are_voids<T, A...>: and_ <
        std::is_void<T>::value,
        are_voids<A...>::value >
{};
template<typename T>
struct are_voids<T> : std::is_void<T> {};


template<typename ... A>
struct are_floating_points;
template<typename T, typename ... A>
struct are_floating_points<T, A...>: and_ <
        std::is_floating_point<T>::value,
        are_floating_points<A...>::value >
{};
template<typename T>
struct are_floating_points<T> : std::is_floating_point<T> {};
template<>
struct are_floating_points<> : std::integral_constant<bool, true> {};


template<typename ... A>
struct are_lvalue_references;
template<typename T, typename ... A>
struct are_lvalue_references<T, A...>: and_ <
        std::is_lvalue_reference<T>::value,
        are_lvalue_references<A...>::value >
{};
template<typename T>
struct are_lvalue_references<T> : std::is_lvalue_reference<T> {};

template<typename T, typename ... A>
struct are_assignable;
template<typename T, typename A0, typename ... As>
struct are_assignable<T, A0, As...>: and_ <
        std::is_assignable<T, A0>::value,
        are_assignable<T, As...>::value >
{};
template<typename T, typename A>
struct are_assignable<T, A> : std::is_assignable<T, A> {};
template<typename T>
struct are_assignable<T> : std::integral_constant<bool, true> {};

// template<typename T, typename ... A>
// struct are_assignable;
// template<typename T, typename A0, typename ... As>
// struct are_assignable<T, A0, As...>: and_ <
//         std::integral_constant<bool, std::is_assignable<T, A0>::value>,
//         std::integral_constant<bool, are_assignable<T, As...>::value >>
// {};
// template<typename T, typename A>
// struct are_assignable<T, A> : std::is_assignable<T, A> {};
// template<typename T>
// struct are_assignable<T> : std::integral_constant<bool, true> {};

// template<typename T, typename ... A>
// struct are_trivially_assignable;
// template<typename T, typename A0, typename ... As>
// struct are_trivially_assignable<T, A0, As...>: and_<
//      std::integral_constant<bool, std::is_trivially_assignable<T, A0>::value>,
//      std::integral_constant<bool, are_trivially_assignable<T, As...>::value>>
// {};
// template<typename T, typename A>
// struct are_trivially_assignable<T, A> : std::is_trivially_assignable<T, A> {};

template<typename T, typename = decltype(std::declval<T>() < std::declval<T>())>
std::true_type supports_less_than_hlp(const T&);
std::false_type supports_less_than_hlp(...);

template<typename T>
using supports_less_than = decltype(supports_less_than_hlp(std::declval<T>()));

/* Check if T provides operator [] */
template<typename T, typename = decltype(std::declval<T>()[0])>
std::true_type is_iterable_hlp(const T&);
std::false_type is_iterable_hlp(...);

template<typename T>
using is_iterable = decltype(is_iterable_hlp(std::declval<T>()));

}

#endif // TYPE_TRAITS_HPP