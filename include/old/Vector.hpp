/*
 * Vector.hpp
 *
 *  Created on: Jan 28, 2014
 *      Author: Thibaut Metivet
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

 #include <array>
 #include <vector>

 #include "Globals.hpp"

 namespace LQCDA {

 	namespace internal {

 		template<typename T, int N>
 		struct VectorTypedef
 		{
 			typedef Eigen::Matrix<T, N, 1> type;
 		};
 		// template<typename T>
 		// struct VectorTypedef<T, 1>
 		// {
 		// 	typedef T type;
 		// };

 		template<typename T, int N>
 		struct vectorTypedef
 		{
 			typedef std::array<T, N> type;
 		};
 		template<typename T>
 		struct vectorTypedef<T, 1>
 		{
 			typedef T type;
 		};
 		template<typename T>
 		struct vectorTypedef<T, Dynamic>
 		{
 			typedef std::vector<T> type;
 		};

 	}

	template<typename T, int N>
 	using Vector = typename internal::VectorTypedef<T, N>::type;
 	
 	template<typename T, int N = Dynamic>
 	using vector = typename internal::vectorTypedef<T, N>::type;


 	namespace internal {

 		template<typename T, int N, int STRIDE>
 		struct fill_helper;

 		template<typename T, int N, int STRIDE>
 		struct fill_helper
 		{
 			template<typename Iterator>
 			static void fill(Vector<T, N> * v, Iterator begin, Iterator end, unsigned int stride = STRIDE) {
 				unsigned int i = 0;
 				while(begin != end) {
 					for(unsigned int s=0; s<stride; ++s)
 						(*v)(i*stride + s) = (*begin)[s];
 					++i; ++begin;
 				}
 			}
 		};
 		template<typename T, int N>
 		struct fill_helper<T, N, 1>
 		{
 			template<typename Iterator>
 			static void fill(Vector<T, N> * v, Iterator begin, Iterator end, unsigned int stride = 1) {
 				unsigned int i = 0;
 				while(begin != end) {
 					(*v)(i++) = *begin++;
 				}
 			}
 		};
 		template<typename T, int N>
 		struct fill_helper<T, N, Dynamic>
 		{
 			template<typename Iterator>
 			static void fill(Vector<T, N> * v, Iterator begin, Iterator end, unsigned int stride) {
 				unsigned int i = 0;
 				while(begin != end) {
 					for(unsigned int s=0; s<stride; ++s)
 						(*v)(i*stride + s) = (*begin)[s];
 					++i; ++begin;
 				}
 			}
 		};

 		template<typename T, int N, typename Iterator, int STRIDE>
 		void fill(Vector<T, N> * v, Iterator begin, Iterator end, int stride =STRIDE)
 		{
 			fill_helper<T, N, STRIDE>::fill(v, begin, end);
 		}
 		template<typename T, int N, typename Iterator>
 		void fill(Vector<T, N> * v, Iterator begin, Iterator end, int stride)
 		{
 			fill_helper<T, N, Dynamic>::fill(v, begin, end, stride);
 		}

 	}

 }

#endif // VECTOR_HPP