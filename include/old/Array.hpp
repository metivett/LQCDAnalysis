/*
 * Array.hpp
 *
 *  Created on: Feb 25, 2014
 *      Author: Thibaut Metivet
 */

#ifndef ARRAY_HPP
#define ARRAY_HPP

 #include "Globals.hpp"

 namespace LQCDA {

 	namespace internal {

 		template<typename T, int N>
 		struct arrayTypedef
 		{
 			typedef std::array<T, N> type;
 		};
 		template<typename T>
 		struct arrayTypedef<T, 1>
 		{
 			typedef T type;
 		};

 		template<typename T, int N, int M>
 		struct ArrayTypedef
 		{
 			typedef Eigen::Array<T, N, M> type;
 		};
 		template<typename T>
 		struct ArrayTypedef<T, 1, 1>
 		{
 			typedef T type;
 		};

 	}

	template<typename T, int N>
 	using array = typename internal::arrayTypedef<T, N>::type;

 	template<typename T, int N, int M = 1>
 	using Array = typename internal::ArrayTypedef<T, N, M>::type;

 }

#endif // ARRAY_HPP