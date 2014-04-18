/*
 * Reduction.hpp
 *
 *  Created on: Apr 09, 2014
 *      Author: Thibaut Metivet
 */

#ifndef REDUCTION_HPP
#define REDUCTION_HPP

 #include "Globals.hpp"
 #include "Exceptions.hpp"
 
 namespace LQCDA {
 	namespace REDUX {
 		namespace internal {
 			template<typename T>
 			struct sum_helper
 			{
 				static T sum(const T& a, const T& b)
 				{
 					return a + b;
 				}
 			};
 			template<typename T>
 			struct prod_helper
 			{
 				static T prod(const T& a, const T& b)
 				{
 					return a * b;
 				}
 			};
 			template<typename T>
 			struct tensorProd_helper
 			{
 				static typename std::enable_if<std::is_fundamental<T>::value, T>::type 
 				tensorProd(const T& a, const T& b)
 				{
 					return a * b;
 				}
 			};

 			template<typename T>
 			struct tensorProd_helper<Vector<T>>
 			{
 				static Matrix<T> tensorProd(const Vector<T>& a, const Vector<T>& b)
 				{
 					return a * b.transpose();
 				}
 			};

 			template<typename Derived>
 			struct tensorProd_helper<Matrix<Derived>>
 			{
 				static Matrix<Derived> tensorProd(const Matrix<Derived>& a, const Matrix<Derived>& b)
 				{
 					if((a.cols() != 1) || (b.cols() != 1))
 					{
 						ERROR(SIZE, "tensor product only valid for column vectors");
 					}
 					return a * b.transpose();
 				}
 			};

 			template<typename Derived>
 			struct tensorProd_helper<Block<Derived>>
 			{
 				static Matrix<Derived> tensorProd(const Block<Derived>& a, const Block<Derived>& b)
 				{
 					if((a.cols() != 1) || (b.cols() != 1))
 					{
 						ERROR(SIZE, "tensor product only valid for column vectors");
 					}
 					return a * b.transpose();
 				}
 			};

 		}

 		template<typename T>
 		auto sum(const T& a, const T& b)
 		-> decltype(internal::sum_helper<T>::sum(a, b))
 		{
 			return internal::sum_helper<T>::sum(a, b);
 		}
 		template<typename T>
 		auto prod(const T& a, const T& b)
 		-> decltype(internal::prod_helper<T>::prod(a, b))
 		{
 			return internal::prod_helper<T>::prod(a, b);
 		}
 		template<typename T>
 		auto tensorProd(const T& a, const T& b)
 		-> decltype(internal::tensorProd_helper<T>::tensorProd(a, b))
 		{
 			return internal::tensorProd_helper<T>::tensorProd(a, b);
 		}

 	}
 }
 
#endif // REDUCTION_HPP