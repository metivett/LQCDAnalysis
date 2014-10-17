/*
 * Function.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <functional>

#include "Globals.hpp"
#include "TypeTraits.hpp"
#include "MetaProgUtils.hpp"
#include "Exceptions.hpp"

BEGIN_NAMESPACE(LQCDA)

/******************************************************************************
 *                               ScalarFunction                               *
 ******************************************************************************/

template<typename Signature>
class StaticScalarFunction;

template<typename T>
class ScalarFunction
{
private:
    unsigned int _xDim;

    template<typename BoundF, typename... Ps>
    StaticScalarFunction<T(Ps...)> bind_hlp(BoundF F, METAPROG::pack<Ps...>) const;

public:
    // Constructors/Destructor
    explicit ScalarFunction(unsigned int xdim)
        : _xDim(xdim)
    {}
    virtual ~ScalarFunction() = default;

    // Accessors
    unsigned int xDim() const
    {
        return _xDim;
    }
    static unsigned int yDim()
    {
        return 1;
    }

    // Evaluators
    virtual T operator()(const T *x) const = 0;
    T operator()(const std::vector<T> &x) const;
    T operator()(const Vector<T> &x) const;
    template<typename... Ts>
    T operator()(const Ts...x) const;

    // Bind
    StaticScalarFunction<T()> bind(const std::vector<T> &x) const;
    StaticScalarFunction<T()> bind(const Vector<T> &x) const;
    template < typename... Args,
               typename = typename std::enable_if <
                   and_< or_<std::is_assignable<T &, Args>::value, (bool)std::is_placeholder<Args>::value>::value... >::value>::type>
    auto bind(Args... args) const
    -> decltype(bind_hlp(std::bind(
                             &ScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                             , this, std::forward<Args>(args)...)
                         , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()));

protected:
    void checkXdim(unsigned int xdim) const;
    void setXDim(unsigned int xdim)
    {
        _xDim = xdim;
    }
};

/******************************************************************************
 *                            StaticScalarFunction                            *
 ******************************************************************************/

template<typename T, typename... Args>
class StaticScalarFunction<T(Args...)>
    : public virtual ScalarFunction<T>
{
    static_assert(are_assignable<T &, Args...>::value,
                  "StaticScalarFunction arguments must be compatible with return type T");
private:
    // Typedefs
    typedef std::function<T(Args...)> fcn_type;

private:
    fcn_type _Fcn;

public:
    StaticScalarFunction(fcn_type f)
        : ScalarFunction<T>(sizeof...(Args))
        , _Fcn(f)
    {}

    using ScalarFunction<T>::operator();
    template<typename... Ts>
    T operator()(const Ts...x) const { 
        static_assert(sizeof...(x) == sizeof...(Args), "wrong number of arguments provided"); 
        return _Fcn(x...); 
    }

protected:
    virtual T operator() (const T *x) const override
    {
        return call_f(x, typename METAPROG::make_int_seq<0, sizeof...(Args)>::type());
    }

private:
    template<std::size_t... s>
    T call_f(const T *x, METAPROG::int_seq<s...>) const
    {
        return _Fcn(std::forward<Args>(const_cast < Args && > (x[s]))...);
    }
};



/******************************************************************************
 *                         ScalarFunction definition                          *
 ******************************************************************************/


template<typename T>
T ScalarFunction<T>::operator()(const std::vector<T> &x) const
{
    checkXdim(x.size());
    return (*this)(x.data());
}
template<typename T>
T ScalarFunction<T>::operator()(const Vector<T> &x) const
{
    checkXdim(x.size());
    return (*this)(x.data());
}
template<typename T>
template<typename... Ts>
T ScalarFunction<T>::operator()(const Ts... xs) const
{
    static_assert(are_assignable<T &, Ts...>::value,
                  "ScalarFunction provided arguments are not compatible with type T");
    // static_assert(are_floating_points<Ts...>::value,
    //               "ScalarFunction provided arguments are not compatible with type T");

    const T x[] = {(T)xs...};
    checkXdim(sizeof...(xs));
    return (*this)(x);
}

template<typename T>
void ScalarFunction<T>::checkXdim(unsigned int xdim) const
{
    if (_xDim && xdim != xDim())
    {
        ERROR(SIZE, "wrong number of arguments provided (expected "
              + utils::strFrom(xDim()) + ", got " + utils::strFrom(xdim) + ")");
    }
}

template<typename T>
StaticScalarFunction<T()> ScalarFunction<T>::bind(const std::vector<T> &x) const
{
    return StaticScalarFunction<T()>
           (std::bind(static_cast<T(ScalarFunction<T>::*)(const T *) const>(&ScalarFunction<T>::operator()), this, x.data())
            , 0);
}

template<typename T>
StaticScalarFunction<T()> ScalarFunction<T>::bind(const Vector<T> &x) const
{
    return StaticScalarFunction<T()>
           (std::bind(static_cast<T(ScalarFunction<T>::*)(const T *) const>(&ScalarFunction<T>::operator()), this, x.data())
            , 0);
}

template<typename T >
template<typename BoundF, typename... Ps>
StaticScalarFunction<T(Ps...)> ScalarFunction<T>::bind_hlp(BoundF F, METAPROG::pack<Ps...>) const
{
    return StaticScalarFunction<T(Ps...)>(F);
}

template<typename T>
template < typename... Args,
           typename>
auto ScalarFunction<T>::bind(Args... args) const
-> decltype(bind_hlp(std::bind(
                         &ScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                         , this, std::forward<Args>(args)...)
                     , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()))
{
    return bind_hlp(
               std::bind(
                   &ScalarFunction<T>::operator()< typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result... >
                   , this, std::forward<Args>(args)...)
               , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result());
}


/******************************************************************************
 *                               VectorFunction                               *
 ******************************************************************************/

// template<typename T>
// class VectorFunction
// {
// private:
//  unsigned int _xDim, _yDim;

// public:
//  // Constructors/Destructor
//  explicit VectorFunction(unsigned int xdim, unsigned int ydim)
//  : _xDim(xdim)
//  , _yDim(ydim)
//  {}
//  virtual ~VectorFunction() = default;

//  // Accessors
//  unsigned int xDim() const { return _xDim; }
//  unsigned int yDim() const { return _yDim; }

//  // Evaluators
//  virtual T operator()(const T * x) const =0;
//  T operator()(const std::vector<T>& x) const;
//  T operator()(const Vector<T>& x) const;
//  template<typename... Ts>
//  T operator()(const Ts...x) const;

// private:
//  void checkXdim(unsigned int xdim) const;

// };

END_NAMESPACE // LQCDA

#endif // FUNCTION_HPP