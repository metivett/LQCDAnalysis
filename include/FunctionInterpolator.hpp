/*
 * FunctionInterpolator.hpp
 *
 *  Created on: Jun 05, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FUNCTION_INTERPOLATOR_HPP_
#define FUNCTION_INTERPOLATOR_HPP_

#include <gsl/gsl_spline.h>

namespace LQCDA {

class FunctionInterpolator
{
private:
gsl_spline * _Spline;
gsl_interp_accel * _Accelerator;

double _LowerBound, _UpperBound;

public:
FunctionInterpolator(double (*f) (double), double a, double b, double step);
FunctionInterpolator(double * x, double * y, unsigned int npts);

~FunctionInterpolator();

double operator() (double x) const;
};

} // namespace LQCDA

#endif	// FUNCTION_INTERPOLATOR_HPP
