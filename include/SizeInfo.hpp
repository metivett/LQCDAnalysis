/*
 * SizeInfo.hpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Thibaut Metivet
 */

#ifndef SIZE_INFO_HPP_
#define SIZE_INFO_HPP_

#include <vector>
#include "Resampled.hpp"

namespace LQCDA {
    
    template<class T>
    unsigned int SizeInfo(const T& t) {
	return 1;
    }

    template<class T>
    unsigned int SizeInfo(const std::vector<T>& t) {
	return t.size();
    }

    template<class T>
    unsigned int SizeInfo(T* t) {
	return SizeInfo(*t);
    }

    template<class T,
	     class Resampler>
    unsigned int SizeInfo(const Resampled<T, Resampler>& t) {
	return SizeInfo(t.Value(0));
    }

} // namespace LQCDA

#endif	// SIZE_INFO_HPP_