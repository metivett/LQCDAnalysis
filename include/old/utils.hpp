/*
 * utils.hpp
 *
 *  Created on: Mar 19, 2013
 *      Author: Thibaut Metivet
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "exceptions.hpp"
#include "Eigen/Dense"

namespace LQCDA {
 

/*
 * Computes the sum of the elements in the sequence between iterators first and last
 */
    template <typename InputIterator>
    inline typename InputIterator::value_type sum (InputIterator first, InputIterator last)
    {
	typename InputIterator::value_type result = *first++;
	for (; first != last; ++first)
	    result = result + *first;
	return result;
    }

/*
 * Computes the mean value of the elements in the sequence between iterators first and last
 */
    template <typename InputIterator>
    inline typename InputIterator::value_type mean (InputIterator first, InputIterator last)
    {
	typename InputIterator::value_type result = *first++;
	int n(1);
	for (; first != last; ++first) {
	    result = result + *first;
	    n++;
	}
	return result/n;
    }


/*
 * Utility functions to manipulate STL vectors
 */
    template<typename T>
    T sum (const std::vector<T>& v)
    {
	return sum(v.begin(), v.end());
    }
    template<typename T>
    T mean (const std::vector<T>& v)
    {
	return mean(v.begin(), v.end());
    }
    template<typename T>
    std::vector<T> operator+ (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're adding vectors of different sizes!");
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] + w[i];
	return result;
    }
    template<typename Ty, class Alloc, typename T>
    std::vector<Ty, Alloc> operator+ (const std::vector<Ty, Alloc>& v, const T& t)
    {
	int n = v.size();
	std::vector<Ty, Alloc> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] + t;
	return result;
    }
    template<typename T>
    std::vector<T> operator- (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're substracting vectors of different sizes!");
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] - w[i];
	return result;
    }
    template<typename Ty, class Alloc, typename T>
    std::vector<Ty> operator- (const std::vector<Ty, Alloc>& v, const T& t)
    {
	int n = v.size();
	std::vector<Ty, Alloc> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] - t;
	return result;
    }
    template<typename T>
    std::vector<T> operator* (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're multiplying vectors of different sizes!");
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] * w[i];
	return result;
    }
    template<typename Ty, class Alloc, typename T>
    std::vector<Ty, Alloc> operator* (const std::vector<Ty, Alloc>& v, const T& t)
    {
	int n = v.size();
	std::vector<Ty, Alloc> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] * t;
	return result;
    }
    template<typename T>
    std::vector<T> operator/ (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're multiplying vectors of different sizes!");
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] / w[i];
	return result;
    }
    template<typename Ty, class Alloc, typename T>
    std::vector<Ty, Alloc> operator/ (const std::vector<Ty, Alloc>& v, const T& t)
    {
	int n = v.size();
	std::vector<Ty, Alloc> result(v);
	for(int i=0; i<n; ++i)
	    result[i] = result[i] / t;
	return result;
    }


} // namespace LQCDA

#endif /* UTILS_HPP_ */
