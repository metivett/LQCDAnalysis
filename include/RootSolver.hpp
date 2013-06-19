/*
 * RootSolver.hpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Thibaut Metivet
 */

#ifndef ROOT_SOLVER_HPP_
#define ROOT_SOLVER_HPP_

#include <gsl/gsl_roots.h>

namespace LQCDA {
/*
template<class Function>
class RootSolver
{
private:
const Function& _Function;
gsl_root_fsolver * _Solver;

public:
RootSolver(const Function& F);
~RootSolver();

double Solve(double x_min, double x_max, double epsrel =1.e-4);

const Function& GetFunction() { return _Function; }
};

template<class Function>
double FindRoot(const Function& F, double x_min, double x_max) {
return RootSolver<Function>(F).Solve(x_min, x_max);
}

template<class Function>
RootSolver<Function>::RootSolver(const Function& F) :
    _Function(F),
    _Solver(gsl_root_fsolver_alloc(gsl_root_fsolver_brent))
{}

    template<class Function>
    RootSolver<Function>::~RootSolver()
{
gsl_root_fsolver_free(_Solver);
}

    template<class Function>
    double RootSolver<Function>::ComputeF(double x, void* params)
    {
return _Function(x);
}

	namespace {
template<class Function>
double gsl_function_wrap(double x, void* params) {
Function* F = (Function*) params;
return (*F)(x);
}
}
	    
	template<class Function>
	double RootSolver<Function>::Solve(double x_min, double x_max, double epsrel)
	{
double r = 0.;
int status;
int iter = 0, max_iter = 100;
double x_lo = x_min;
double x_hi = x_max;

gsl_function fun;
fun.function = &gsl_function_wrap<Function>;
fun.params = 0;

gsl_root_fsolver_set(_Solver, &fun, x_lo, x_hi);

do {
iter++;
status = gsl_root_fsolver_iterate(_Solver);
r = gsl_root_fsolver_root(_Solver);

x_lo = gsl_root_fsolver_x_lower(_Solver);
x_hi = gsl_root_fsolver_x_upper(_Solver);
status = gsl_root_test_interval(x_lo, x_hi, 0., epsrel);

} while(status == GSL_CONTINUE && iter < max_iter);

return r;
}*/

} // namespace LQCDA

#endif	// ROOT_SOLVER_HPP
