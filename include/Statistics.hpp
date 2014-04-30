/*
 * Statistics.hpp
 *
 *  Created on: Apr 24, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

 #include "Globals.hpp"
 #include "Reduction.hpp"

 namespace LQCDA {

 	template<typename Derived>
 	typename MatrixExpr<Derived>::Scalar mean(const MatrixExpr<Derived>& mat)
 	{
 		typedef typename MatrixExpr<Derived>::Scalar Scalar;
 		Scalar res;
 		res = mat.redux(&REDUX::sum<Scalar>);

 		return res;
 	}

 	template<typename Derived>
 	typename MatrixExpr<Derived>::Scalar variance(const MatrixExpr<Derived>& mat)
 	{
 		typedef typename MatrixExpr<Derived>::Scalar Scalar;
 		Scalar res;
 		double len = mat.size();
 		auto mat_sum = mat.redux(&REDUX::sum<Scalar>);

 		res = ( mat.binaryExpr(mat, &REDUX::prod<Scalar>).redux(&REDUX::sum<Scalar>) 
 				- REDUX::prod(mat_sum, mat_sum) / (len) )
 				/ (len - 1);

 		return res;
 	}

 }

#endif // STATISTICS_HPP