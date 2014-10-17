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

BEGIN_NAMESPACE(LQCDA)

template<typename T>
class StaticScalarFunction
    : public virtual ScalarFunction<T>
{
private:
    // Typedefs
    typedef std::function<T(const T *)> fcn_type;

private:
    fcn_type _Fcn;

public:
    StaticScalarFunction(fcn_type f, unsigned int xdim)
        : ScalarFunction<T>(xdim)
        , _Fcn(f)
    {}

private:
    virtual T operator() (const T *x) const override
    {
        return _Fcn(x);
    }
};

END_NAMESPACE // LQCDA

#endif // STATIC_FUNCTION_HPP