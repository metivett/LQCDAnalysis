/*
 * CovariancePolicies.hpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Thibaut Metivet
 */

#ifndef GET_VALUE_HPP_
#define GET_VALUE_HPP_

namespace LQCDA {

template<class U, class T>
U GetValue(const T& t, unsigned int i) {
return static_cast<U>(t[i]);
}

template<class U>
U GetValue(const double& t, unsigned int i) {
assert(i == 0);
return t;
}

} // namespace LQCDA

#endif	// GET_VALUE_HPP_
