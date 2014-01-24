/*
 * TypeTraits.hpp
 *
 *  Created on: Dec 11, 2013
 *      Author: Thibaut Metivet
 */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>

namespace LQCDA {

template<class T, class U> struct and_;
template<class T, class U, T t, U u>
struct and_<std::integral_constant<T,t>, std::integral_constant<U,u>>: std::integral_constant<bool, t && u> {};

template<class T, class U> struct or_;
template<class T, class U, T t, U u>
struct or_<std::integral_constant<T,t>, std::integral_constant<U,u>>: std::integral_constant<bool, t || u> {};

template<class T> struct not_;
template<class T, T t>
struct not_<std::integral_constant<T,t>>: std::integral_constant<bool, !t> {};

template<bool Condition, typename T, typename F> struct if_;
template<typename T, typename F>
struct if_<true, T, F> { typedef T result; };
template<typename T, typename F>
struct if_<false, T, F> { typedef F result; };


template<typename ... A>
struct are_voids;
template<typename T, typename ... A>
struct are_voids<T, A...>: and_<
		std::integral_constant<bool, std::is_void<T>::value>, 
		std::integral_constant<bool, are_voids<A...>::value>>
{};
template<typename T>
struct are_voids<T> : std::is_void<T> {};


template<typename ... A>
struct are_floating_points;
template<typename T, typename ... A>
struct are_floating_points<T, A...>: and_<
		std::integral_constant<bool, std::is_floating_point<T>::value>, 
		std::integral_constant<bool, are_floating_points<A...>::value>>
{};
template<typename T>
struct are_floating_points<T> : std::is_floating_point<T> {};


template<typename ... A>
struct are_lvalue_references;
template<typename T, typename ... A>
struct are_lvalue_references<T, A...>: and_<
		std::integral_constant<bool, std::is_lvalue_reference<T>::value>, 
		std::integral_constant<bool, are_lvalue_references<A...>::value>>
{};
template<typename T>
struct are_lvalue_references<T> : std::is_lvalue_reference<T> {};

}

#endif // TYPE_TRAITS_HPP