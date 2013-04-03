/*
 * correlator_io.hpp
 *
 *  Created on: Feb 15, 2013
 *      Author: Thibaut Metivet
 */

#ifndef CORRELATOR_IO_HPP_
#define CORRELATOR_IO_HPP_

#include <string>
#include "interpolator.hpp"

namespace LQCDA {

void readBaseCorrelator(Eigen::ArrayXd& c, const Interpolator& i, const std::string& filename, IOFormat format);

}	// namespace LQCDA


#endif /* CORRELATOR_IO_HPP_ */
