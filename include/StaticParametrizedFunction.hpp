/*
 * StaticParametrizedFunction.hpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Thibaut Metivet
 */

#ifndef STATIC_PARAMETRIZED_FUNCTION_HPP
#define STATIC_PARAMETRIZED_FUNCTION_HPP

 #include "StaticFunction.hpp"
 #include "ParametrizedFunction.hpp"
 #include "TypeTraits.hpp"
 #include "MetaProgUtils.hpp"

namespace LQCDA {

	template<typename XT, typename YT, typename... ARGS>
 	class StaticParametrizedFunction: public ParametrizedFunction<XT, YT, sizeof...(ARGS)>
 	{
 	private:
        INHERIT_FUNCTION_TYPEDEFS;

 		y_type (*_Fcn)(x_arg_type, ARGS...);

 	public:
 		StaticParametrizedFunction(y_type (*f)(x_arg_type, ARGS...)) : _Fcn(f)
 		{
 			static_assert(are_floating_points<ARGS...>::value, 
 				"The parameters of the function must be floating-point.");
 		}

 	protected:
 		virtual y_type eval(x_arg_type x, const std::array<double, sizeof...(ARGS)>& p) {
 			return apply_(index_bind<1>(_Fcn, x), p);
 		}
 	};

}

#endif // STATIC_PARAMETRIZED_FUNCTION_HPP