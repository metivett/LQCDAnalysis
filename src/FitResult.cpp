/*
 * FitResult.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#include "FitResult.hpp"

std::ostream& LQCDA::operator<< (std::ostream& out, const FitResult& res)
{
    out << "\nFitted parameters:\n";
    out << res.FittedParameters()
	<< '\n';

    return out;
}
