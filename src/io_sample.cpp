/*
 * io_sample.cpp
 *
 *  Created on: Mar 5, 2013
 *      Author: Thibaut Metivet
 */

#include <istream>
#include <iostream>
#include "io_sample.hpp"
#include "io_correlator.hpp"

std::vector<Eigen::ArrayXd> LQCDA::readCorrSample (const char* fname, const Interpolator& i, IOFormat format)
{
    std::vector<Eigen::ArrayXd> result;
    result.reserve(500);
    switch (format) {
    case LATAN_1:
	std::ifstream ifs(fname);
	std::string file;
	while (getline(ifs, file)) {
	    Eigen::ArrayXd c;
	    readBaseCorrelator(c, i, file, format);
	    result.push_back(c);
	}
	break;
    }
    return result;
}

template<typename Scalar>
std::vector<Scalar> LQCDA::readScalarSample (const char* fname, IOFormat format)
{
    std::vector<Scalar> result;
    result.reserve(500);
    switch (format) {
    case LATAN_1:
	std::ifstream ifs(fname);
	std::string file;
	while (getline(ifs, file)) {
	    Scalar c;
	    AsciiFileReader reader(file);
	    reader >> c;
	    result.push_back(c);
	}
	break;
    }
    return result;
}
