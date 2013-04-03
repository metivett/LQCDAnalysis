/*
 * globals.hpp
 *
 *  Created on: Feb 11, 2013
 *      Author: Thibaut Metivet
 */

#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_

namespace LQCDA {

/**
 * SFINAE tests
 */
// Compile-checks whether class T defines typedef t_def
#define HAS_TYPEDEF(T, t_def) \
{\
	template <typename T> \
	struct has_typedef_##t_def {\
	typedef char yes[1];\
	typedef char no[2];\
	template <typename C> \
	static yes& test(typename C::##t_def *);\
	template <typename> \
	static no& test(...);\
	static const bool value = sizeof(test<T>(0)) == sizeof(yes);\
	}; \
}


}	//namespace LQCDA

#endif /* GLOBALS_HPP_ */
