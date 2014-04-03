/*
 * Matrix.hpp
 *
 *  Created on: Feb 12, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP

 #include "Globals.hpp"

 namespace LQCDA {

 	namespace internal {

 		template<typename T, int N, int M>
 		struct MatrixTypedef
 		{
 			typedef Eigen::Matrix<T, N, M> type;
 		};
 		template<typename T>
 		struct MatrixTypedef<T, 1, 1>
 		{
 			typedef T type;
 		};

 	}

	template<typename T, int N, int M>
 	using Matrix = typename internal::MatrixTypedef<T, N, M>::type;

 }

#endif // MATRIX_HPP