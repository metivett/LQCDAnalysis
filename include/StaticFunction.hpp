/*
 * StaticFunction.hpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STATIC_FUNCTION_HPP
#define STATIC_FUNCTION_HPP

 #include "Function.hpp"
 #include "MetaProgUtils.hpp"
 #include <functional>

 namespace LQCDA {

	template<typename Scalar, int XDIM, int YDIM>
 	class StaticFunction: public virtual Function<Scalar, XDIM, YDIM>
 	{
 	INHERIT_FUNCTION_TYPEDEFS;

 	private:
 		// Typedefs
 		typedef Function<Scalar, XDIM, YDIM> Base;
 		typedef std::function<y_type(arg_type)> fcn_type;

 	private:
 		fcn_type _Fcn;

 	public:
 		StaticFunction(fcn_type f) : _Fcn(f) {}

 	private:
 		virtual y_type operator() (arg_type x) const override {
 			return _Fcn(x);
 		}
 	};

 }

#endif // STATIC_FUNCTION_HPP