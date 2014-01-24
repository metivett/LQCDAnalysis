/*
 * io_sample.hpp
 *
 *  Created on: Mar 5, 2013
 *      Author: Thibaut Metivet
 */

#ifndef IO_SAMPLE_HPP_
#define IO_SAMPLE_HPP_

#include <vector>
#include "Eigen/Dense"
#include "io.hpp"
#include "interpolator.hpp"

namespace LQCDA {

/*
 * Utility functions
 */
// Construct from manifest file
std::vector<Eigen::ArrayXd> readCorrSample (const char* fname, const Interpolator& i, IOFormat format = LATAN_1);

template<typename Scalar>
std::vector<Scalar> readScalarSample (const char* fname, IOFormat format = LATAN_1);

}	// namespace LQCDA


#endif /* IO_SAMPLE_HPP_ */
