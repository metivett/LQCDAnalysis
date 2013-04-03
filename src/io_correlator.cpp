/*
 * io_correlator.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: Thibaut Metivet
 */

#include <algorithm>
#include <istream>
#include <iostream>
#include "io_correlator.hpp"
#include "io.hpp"

namespace LQCDA {

void readBaseCorrelator(Eigen::ArrayXd& c, const Interpolator& i, const std::string& filename, IOFormat format)
{
	AsciiFileReader reader(filename);
	std::string instr;
	while(getNextInstruction(reader, instr, format))
	{
		// remove blank spaces in the instruction line
		instr.erase(std::remove_if(instr.begin(), instr.end(), isspace));
		std::string interpol = i.str(format);
		// compare with the "stringified" interpolator
		if (instr.compare(1, interpol.size(), interpol) == 0)
		{
			readVector(reader, c);
			return;
		}
		else
			continue;
	}
	std::cerr << "No instruction found in file :" << filename;
}

}	// namespace LQCDA


