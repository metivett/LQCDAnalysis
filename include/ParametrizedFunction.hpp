/*
 * ParametrizedFunction.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PARAMETRIZED_FUNCTION_HPP
#define PARAMETRIZED_FUNCTION_HPP

#include <functional>

#include "Function.hpp"

BEGIN_NAMESPACE(LQCDA)

/******************************************************************************
 *                       Scalar Parametrized Function                         *
 ******************************************************************************/

// Generic scalar ParametrizedFunction
template<typename T>
class ParametrizedScalarFunction
{
public: // Constructors/Destructor
    explicit ParametrizedScalarFunction(const unsigned int xdim = 0,
                                        const unsigned int npar = 0);
    virtual ~ParametrizedScalarFunction() = default;

protected: // Assignment
    void setSize(const unsigned int xdim, const unsigned int npar);

public: // Queries
    virtual unsigned int xDim() const
    {
        return m_xDim;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }
    virtual unsigned int nPar() const
    {
        return m_nPar;
    }

public: // Evaluator
    virtual T operator()(const T *x, const T *p) const = 0;
    T operator()(const std::vector<T> &x, const std::vector<T> &p) const;
    T operator()(const Vector<T> &x, const Vector<T> &p) const;

private: // Utility functions
    void checkXdim(unsigned int xdim) const;
    void checkParIndex(unsigned int i) const;
    void checkNpar(unsigned int n) const;

private: // Data
    unsigned int m_xDim;
    unsigned int m_nPar;
};

// Dynamic scalar ParametrizedFunction
template<typename T>
class ParametrizedSFunction
    : public ParametrizedScalarFunction<T>
{
private: // Typedefs
    typedef std::function<T(const T *, const T *)> function_type;

public: // Constructors/Destructor
    explicit ParametrizedSFunction(const unsigned int xdim = 0,
                                        const unsigned int npar = 0,
                                        const function_type &f = nullptr);
    virtual ~ParametrizedSFunction() = default;

public: // Assignment
    void setFunction(const function_type &f, const unsigned int xdim, const unsigned int npar);

public: // Queries
    using ParametrizedScalarFunction<T>::xDim;
    using ParametrizedScalarFunction<T>::yDim;
    using ParametrizedScalarFunction<T>::nPar;

public: // Evaluator
    virtual T operator()(const T *x, const T *p) const override;
    using ParametrizedScalarFunction<T>::operator();

private: // Data
    function_type m_f;
};


/******************************************************************************
 *                     Bind parameters utility functions                      *
 ******************************************************************************/

BEGIN_NAMESPACE(internal)

// Parametrized Scalar function bind
// template<typename T>
// struct BindHelper<ParametrizedSFunction<T>, const std::vector<T> &>
// {
//     typedef ParametrizedSFunction<T> ReturnType;
//     typedef SFunction<T> BindParReturnType;

//     static BindParReturnType bindParameters(ParametrizedSFunction<T> &&f, const std::vector<T> &p)
//     {
//         auto f_caller = [f](const T* _x, const std::vector<T> &_p){return f(_x, _p.data());};
//         auto f_bind = std::bind(f_caller, std::placeholders::_1, p);
//         return BindParReturnType(f.xDim(), f_bind);
//     }
// };
// template<typename T>
// struct BindHelper<ParametrizedSFunction<T>, const Vector<T> &>
// {
//     typedef ScalarFunction<T()> ReturnType;
//     static ReturnType bind(ScalarFunction<T> &&f, const Vector<T> &x)
//     {
//         using f_op = T(ScalarFunction<T>::*)(const T *) const;
//         return ReturnType(std::bind(static_cast<f_op>(ScalarFunction<T>::operator()), f, x.data()));
//     }
// };

END_NAMESPACE // internal

/******************************************************************************
 *                         StaticParametrizedFunction                         *
 ******************************************************************************/

// template<typename T, typename... Args>
// class StaticParametrizedScalarFunction<T(Args...)>
//     : public virtual ParametrizedScalarFunction<T>
// {
//     static_assert(are_assignable<T &, Args...>::value,
//                   "StaticParametrizedScalarFunction arguments must be compatible with return type T");

// private:
//     // Typedefs
//     typedef std::function<T(Args...)> fcn_type;

//     fcn_type m_Fcn;

// public:
//     // Constructors/Destructor
//     StaticParametrizedScalarFunction(fcn_type f, unsigned int npar)
//         : ParametrizedScalarFunction<T>(sizeof...(Args) - npar, npar)
//         , m_Fcn(f)
//     {}
//     virtual ~StaticParametrizedScalarFunction() noexcept = default;

//     // Accessors
//     using ParametrizedScalarFunction<T>::xDim;
//     using ParametrizedScalarFunction<T>::yDim;
//     using ParametrizedScalarFunction<T>::nPar;

//     // Evaluators
//     using ParametrizedScalarFunction<T>::operator();

//     // Bind
//     using ParametrizedScalarFunction<T>::bind;

// protected:
//     virtual T operator()(const T *x, const T *p) const override
//     {
//         T *args = new T[sizeof...(Args)];
//         std::copy(x, x + xDim(), args);
//         std::copy(p, p + nPar(), args + xDim());
//         T result = call_f(args, typename METAPROG::make_int_seq<0, sizeof...(Args)>::type());
//         delete[] args;
//         return result;
//     }

// private:
//     template<std::size_t... s>
//     T call_f(const T *args, METAPROG::int_seq<s...>) const
//     {
//         return m_Fcn(std::forward<Args>(const_cast < Args && > (args[s]))...);
//     }

// };


/******************************************************************************
 *                   ParametrizedScalarFunction<T> definition                 *
 ******************************************************************************/

template<typename T>
ParametrizedScalarFunction<T>::ParametrizedScalarFunction(
    const unsigned int xdim,
    const unsigned int npar)
{
    setSize(xdim, npar);
}

template<typename T>
void ParametrizedScalarFunction<T>::setSize(const unsigned int xdim, const unsigned int npar)
{
    m_xDim = xdim;
    m_nPar = npar;
}

template<typename T>
T ParametrizedScalarFunction<T>::operator()(const std::vector<T> &x, const std::vector<T> &p) const
{
    this->checkXdim(x.size());
    this->checkNpar(p.size());
    return (*this)(x.data(), p.data());
}
template<typename T>
T ParametrizedScalarFunction<T>::operator()(const Vector<T> &x, const Vector<T> &p) const
{
    this->checkXdim(x.size());
    this->checkNpar(p.size());
    return (*this)(x.data(), p.data());
}

template<typename T>
void ParametrizedScalarFunction<T>::checkXdim(unsigned int xdim) const
{
    if (m_xDim && xdim != xDim())
    {
        ERROR(SIZE, "wrong number of arguments provided (expected "
              + utils::strFrom(xDim()) + ", got " + utils::strFrom(xdim) + ")");
    }
}

template<typename T>
void ParametrizedScalarFunction<T>::checkParIndex(unsigned int i) const
{
    if (i >= m_nPar)
    {
        ERROR(SIZE, "out of limit parameter (requested "
              + utils::strFrom(i) + " out of " + utils::strFrom(m_nPar) + ")");
    }
}

template<typename T>
void ParametrizedScalarFunction<T>::checkNpar(unsigned int n) const
{
    if (n != m_nPar)
    {
        ERROR(SIZE, "wrong number of parameters provided (expected "
              + utils::strFrom(m_nPar) + ", got " + utils::strFrom(n) + ")");
    }
}

/******************************************************************************
 *                     ParametrizedSFunction<T> definition                    *
 ******************************************************************************/

template<typename T>
ParametrizedSFunction<T>::ParametrizedSFunction(
    const unsigned int xdim,
    const unsigned int npar,
    const function_type &f)
{
    setFunction(f, xdim, npar);
}

template<typename T>
void ParametrizedSFunction<T>::setFunction(const function_type &f, const unsigned int xdim, const unsigned int npar)
{
    ParametrizedScalarFunction<T>::setSize(xdim, npar);
    m_f = f;
}

template<typename T>
T ParametrizedSFunction<T>::operator()(const T *x, const T *p) const
{
    return m_f(x, p);
}
// template<typename T>
// template<typename... Ts>
// T ParametrizedScalarFunction<T>::operator()(const Ts...a) const
// {
//     static_assert(are_assignable<T &, Ts...>::value,
//                   "ParametrizedScalarFunction provided arguments are not compatible with type T");
//     const T args[] = {(T)a...};
//     this->checkXdim(sizeof...(a) - m_nPar);
//     return (*this)(args, args + xDim());
// }

// template<typename T>
// StaticScalarFunction<T()> ParametrizedScalarFunction<T>::bind(const std::vector<T> &x, const std::vector<T> &p) const
// {
//     return StaticScalarFunction<T()>
//            (std::bind(
//                 static_cast<T(ParametrizedScalarFunction<T>::*)(const T *, const T *) const>(&ParametrizedScalarFunction<T>::operator())
//                 , this
//                 , x.data(), p.data()));
// }

// template<typename T>
// StaticScalarFunction<T()> ParametrizedScalarFunction<T>::bind(const Vector<T> &x, const Vector<T> &p) const
// {
//     return StaticScalarFunction<T()>
//            (std::bind(
//                 static_cast<T(ParametrizedScalarFunction<T>::*)(const T *, const T *) const>(&ParametrizedScalarFunction<T>::operator())
//                 , this
//                 , x.data(), p.data()));
// }

// template<typename T >
// template<typename BoundF, typename... Ps>
// StaticScalarFunction<T(Ps...)> ParametrizedScalarFunction<T>::bind_hlp(BoundF F, METAPROG::pack<Ps...>) const
// {
//     return StaticScalarFunction<T(Ps...)>(F);
// }

// template<typename T>
// template < typename... Args,
//            typename>
// auto ParametrizedScalarFunction<T>::bind(Args... args) const
// -> decltype(bind_hlp(std::bind(
//                          &ParametrizedScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
//                          , this, std::forward<Args>(args)...)
//                      , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()))
// {
//     return bind_hlp(
//                std::bind(
//                    &ParametrizedScalarFunction<T>::operator()< typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
//                    , this, std::forward<Args>(args)...)
//                , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result());
// }




END_NAMESPACE // LQCDA

#endif // PARAMETRIZED_FUNCTION_HPP