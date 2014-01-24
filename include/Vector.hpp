/*
 * Vector.hpp
 *
 *  Created on: Jan 28, 2014
 *      Author: Thibaut Metivet
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

 #include "Globals.hpp"

 namespace LQCDA {

 	namespace internal {

 		template<typename T, int N>
 		struct VectorTypedef
 		{
 			typedef Eigen::Matrix<T, N, 1> type;
 		};
 		template<typename T>
 		struct VectorTypedef<T, 1>
 		{
 			typedef T type;
 		};

 	}

	template<typename T, int N>
 	using Vector = typename internal::VectorTypedef<T, N>::type;
 	
 	template<typename T>
 	using vector = typename std::vector<T>;

 }

#endif // VECTOR_HPP