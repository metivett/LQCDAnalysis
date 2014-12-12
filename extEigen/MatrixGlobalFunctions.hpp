/*
 * MatrixGlobalFunctions.hpp
 *
 *  Created on: Dec 10, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MATRIX_GLOBAL_FUNCTIONS_HPP
#define MATRIX_GLOBAL_FUNCTIONS_HPP

#define EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(NAME,FUNCTOR) \
  template<typename Derived> \
  inline const Eigen::CwiseUnaryOp<Eigen::internal::FUNCTOR<typename Derived::Scalar>, const Derived> \
  NAME(const Eigen::MatrixBase<Derived>& x) { \
    return x.derived(); \
  }

namespace Eigen
{

// EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(real,scalar_real_op)
// EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(imag,scalar_imag_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(conj,scalar_conjugate_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(sin,scalar_sin_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(cos,scalar_cos_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(asin,scalar_asin_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(acos,scalar_acos_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(tan,scalar_tan_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(exp,scalar_exp_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(log,scalar_log_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(abs,scalar_abs_op)
EIGEN_MATRIX_DECLARE_GLOBAL_UNARY(sqrt,scalar_sqrt_op)

}

#endif // MATRIX_GLOBAL_FUNCTIONS_HPP
