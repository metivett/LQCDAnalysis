/*
 * FunctionInterpolator.cpp
 *
 *  Created on: Jun 05, 2013
 *      Author: Thibaut Metivet
 */

#include "FunctionInterpolator.hpp"
#include <assert.h>

LQCDA::FunctionInterpolator::FunctionInterpolator(double (*f) (double), double a, double b, double step) :
    _Accelerator(gsl_interp_accel_alloc()),
    _Spline(gsl_spline_alloc(gsl_interp_cspline, (int)((b - a) / step))),
    _LowerBound(a),
    _UpperBound(b)
{
    assert(_LowerBound < _UpperBound);
    unsigned int npts = (b - a) / step;
    double x[npts], y[npts];
    for(int i = 0; i < npts; ++i) {
	x[i] = a + i * step;
	y[i] = f(x[i]);
    }

    gsl_spline_init(_Spline, x, y, npts);
}

LQCDA::FunctionInterpolator::FunctionInterpolator(double * x, double * y, unsigned int npts) :
    _Accelerator(gsl_interp_accel_alloc()),
    _Spline(gsl_spline_alloc(gsl_interp_cspline, npts)),
    _LowerBound(x[0]),
    _UpperBound(x[npts-1])
{
    assert(_LowerBound < _UpperBound);
    gsl_spline_init(_Spline, x, y, npts);
}

LQCDA::FunctionInterpolator::~FunctionInterpolator()
{
    gsl_spline_free(_Spline);
    gsl_interp_accel_free(_Accelerator);
}

double LQCDA::FunctionInterpolator::operator() (double x) const
{
    assert(_LowerBound <= x && x <= _UpperBound);
    return gsl_spline_eval(_Spline, x, _Accelerator);
}
