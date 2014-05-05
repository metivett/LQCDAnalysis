/*
 * GSLRootFinder.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: Thibaut Metivet
 */

#ifndef GSL_ROOT_FINDER_HPP
#define GSL_ROOT_FINDER_HPP

 #include "RootFinder.hpp"

 #include <gsl/gsl_errno.h>
 #include <gsl/gsl_roots.h>

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(Roots)

 template<typename T>
 class GSLRootFinder
 : public RootFinder<T>
 {
 protected:
 	std::unique_ptr<gsl_root_fsolver, decltype(&gsl_root_fsolver_free)> _Solver{nullptr, nullptr};

 	//Constructor
 	GSLRootFinder(gsl_root_fsolver* solver);

 public:
 	// Pure virtual destructor
 	virtual ~GSLRootFinder() =0;

 private:
 	virtual Root<T> solve_h(const ScalarFunction<T>& f, T xmin, T xmax, double epsrel) override;
 };

 template<typename T>
 GSLRootFinder<T>::GSLRootFinder(gsl_root_fsolver* solver)
 : _Solver(solver)
 {}

 template<typename T>
 GSLRootFinder<T>::~GSLRootFinder<T>() {}

 template<typename T>
 Root<T> GSLRootFinder<T>::solve_h(const ScalarFunction<T>& f, T xmin, T xmax, double epsrel)
 {
 	double r = 0.;
 	int status;
 	int iter = 0, max_iter = 100;
 	double x_lo = xmin;
 	double x_hi = xmax;

 	gsl_function fun;
 	fun.function = [](double x, void * p)->double
 		{
 			return (*static_cast<const ScalarFunction<T>*>(p))(x);
 		};
 	fun.params = &f;

 	gsl_root_fsolver_set(_Solver.get(), &fun, x_lo, x_hi);

 	do {
 		iter++;
 		status = gsl_root_fsolver_iterate(_Solver.get());
 		r = gsl_root_fsolver_root(_Solver.get());

 		x_lo = gsl_root_fsolver_x_lower(_Solver.get());
 		x_hi = gsl_root_fsolver_x_upper(_Solver.get());
 		status = gsl_root_test_interval(x_lo, x_hi, 0., epsrel);

 	} while(status == GSL_CONTINUE && iter < max_iter);

 	return Root<T>(r, x_hi-x_lo);
 }

 template<typename T>
 class BrentRootFinder
 : public GSLRootFinder<T>
 {
 public:
 	// Constructor/Destructor
 	BrentRootFinder();
 	virtual ~BrentRootFinder() = default;
 };

 template<typename T> 
 BrentRootFinder<T>::BrentRootFinder()
 : GSLRootFinder<T>(gsl_root_fsolver_alloc(gsl_root_fsolver_brent), gsl_root_fsolver_free)
 {}


 END_NAMESPACE
 END_NAMESPACE

#endif // GSL_ROOT_FINDER_HPP