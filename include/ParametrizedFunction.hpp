/*
 * ParametrizedFunction.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PARAMETRIZED_FUNCTION_HPP
#define PARAMETRIZED_FUNCTION_HPP

#include "Function.hpp"

BEGIN_NAMESPACE(LQCDA)

/******************************************************************************
 *                           Parametrized Function                            *
 ******************************************************************************/

template<typename T>
class ParametrizedScalarFunction
    : public ScalarFunction<T>
{
private:
    unsigned int _nPar;
    std::vector<T> _P;

public:
    // Constructors/Destructor
    explicit ParametrizedScalarFunction(unsigned int xdim, unsigned int npar)
        : ScalarFunction<T>(xdim)
        , _nPar(npar)
        , _P(npar)
    {}
    explicit ParametrizedScalarFunction(unsigned int xdim, const std::vector<T> &p)
        : ScalarFunction<T>(xdim)
        , _nPar(p.size())
        , _P(p)
    {}
    virtual ~ParametrizedScalarFunction() noexcept = default;

    // Accessors
    unsigned int nPar() const
    {
        return _nPar;
    }
    T getParValue(unsigned int i) const
    {
        checkParIndex(i);
        return _P[i];
    }
    const std::vector<T> getParValues() const
    {
        return _P;
    }

    // Setters
    void setParameter(unsigned int i, T p)
    {
        checkParIndex(i);
        _P[i] = p;
    }
    void setParameters(const std::vector<T> &p)
    {
        checkNpar(p.size());
        _P = p;
    }

    // Evaluators
    virtual T operator()(const T *x) const;
    virtual T operator()(const T *x, const T *p) const = 0;

private:
    void checkParIndex(unsigned int i) const;
    void checkNpar(unsigned int n) const;

};

template<typename T>
T ParametrizedScalarFunction<T>::operator()(const T *x) const
{
    return (*this)(x, _P.data());
}

template<typename T>
void ParametrizedScalarFunction<T>::checkParIndex(unsigned int i) const
{
    if (i >= _nPar)
    {
        ERROR(SIZE, "out of limit parameter (requested "
              + utils::strFrom(i) + " out of " + utils::strFrom(_nPar) + ")");
    }
}

template<typename T>
void ParametrizedScalarFunction<T>::checkNpar(unsigned int n) const
{
    if (n != _nPar)
    {
        ERROR(SIZE, "wrong number of parameters provided (expected "
              + utils::strFrom(_nPar) + ", got " + utils::strFrom(n) + ")");
    }
}


END_NAMESPACE // LQCDA

#endif // PARAMETRIZED_FUNCTION_HPP