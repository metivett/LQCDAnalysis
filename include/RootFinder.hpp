/*
 * RootFinder.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: Thibaut Metivet
 */

#ifndef ROOT_FINDER_HPP
#define ROOT_FINDER_HPP

 #include "Globals.hpp"
 #include "Exceptions.hpp"
 #include "Function.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(Roots)

 template<typename T>
 class Root
 {
 private:
 	// Root value and error
 	T _r, _e;

 public:
 	Root(T r, T e)
 	: _r{r}
 	, _e{e}
 	{}

 	T value() const { return _r; }
 	T error() const { return _e; }
 };
 
 template<typename T>
 class RootFinder
 {
 public:
 	// Constructors/Destructor
 	RootFinder() = default;
 	virtual ~RootFinder() =0;

 	Root<T> solve(const ScalarFuntion<T>& f, T xmin, T xmax);

 private:
 	virtual Root<T> solve_h(const ScalarFuntion<T>& f, T xmin, T xmax) =0;
 };

 template<typename T>
 RootFinder<T>::~RootFinder<T>() {}

 template<typename T>
 Root<T> RootFinder<T>::solve(const ScalarFuntion<T>& f, T xmin, T xmax)
 {
 	if(f.xDim() != 1)
 	{
 		ERROR(SIZE, "root finding only accepts one-dimensional functions");
 	}
 	return solve_h(f, xmin, xmax);
 }

 END_NAMESPACE
 END_NAMESPACE

#endif // ROOT_FINDER_HPP