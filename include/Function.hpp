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
 *                                  Function                                  *
 ******************************************************************************/

// Generic Function template
template<typename S, int... DIMS>
class Function;

// Generic BindReturnTypeHelper
template<typename S, int... DIMS>
struct BindReturnTypeHelper;

// Forward declarations
// Scalar Function
template<typename T, unsigned int XDIM>
class Function<T, XDIM, 1>;
template<typename T>
class Function<T, Dynamic, 1>;
template<typename T, typename... ARGS>
class Function<T(ARGS...)>;
// BindReturnTypeHelper
template<typename T, unsigned int XDIM>
struct BindReturnTypeHelper<T, XDIM, 1>;
template<typename T>
struct BindReturnTypeHelper<T, Dynamic, 1>;
template<typename T, typename... ARGS>
struct BindReturnTypeHelper<T(ARGS...)>;


// Static scalar function specialization
template<typename T, unsigned int XDIM>
class Function<T, XDIM, 1>
{
private:
    // Metaprog utility struct for bind arguments
    template<typename U, typename... Args>
    struct are_assignable_or_placeholders
    : and_< or_<std::is_assignable<U &, Args>::value, (bool)std::is_placeholder<Args>::value>::value ... >
    {};
    // bind helper function
    template<typename BoundF, typename... Ps>
    Function<T(Ps...)> bind_hlp(BoundF F, METAPROG::pack<Ps...>) const;

public:
    // Constructors/Destructor
    Function() = default;
    virtual ~Function() = default;

    // Accessors
    static constexpr unsigned int xDim()
    {
        return XDIM;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }

    // Evaluators
    virtual T operator()(const T *x) const = 0;
    T operator()(const std::vector<T> &x) const;
    T operator()(const Vector<T> &x) const;
    template<typename... Ts>
    T operator()(const Ts...x) const;

    // Binders
    Function<T()> bind(const std::vector<T> &x) const;
    Function<T()> bind(const Vector<T> &x) const;
    template < typename... Args,
               typename = typename std::enable_if <are_assignable_or_placeholders<T, Args...>::value>::type>
    BindReturnTypeHelper<T, METAPROG::pack_count_placeholders<Args...>::value, 1> bind(Args... args) const;
    // template < typename... Args,
    //            typename = typename std::enable_if <are_assignable_or_placeholders<T, Args...>::value>::type>
    // auto bind(Args... args) const
    // -> decltype(
    //     bind_hlp(
    //         std::bind(
    //             &Function<T, XDIM, 1>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
    //             , this, std::forward<Args>(args)...
    //         )
    //         , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()
    //     ));

};

// Dynamic scalar function specialization
template<typename T>
class Function<T, Dynamic, 1>
{
public:
    // Constructors/Destructor
    Function(unsigned int xdim)
        : m_xDim(xdim)
    {}
    virtual ~Function() = default;

    // Accessors
    unsigned int xDim() const
    {
        return m_xDim;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }

    // Evaluators
    virtual T operator()(const T *x) const = 0;
    T operator()(const std::vector<T> &x) const;
    T operator()(const Vector<T> &x) const;

private:
    // check xdim == m_xDim
    void checkXdim(unsigned int xdim) const;

private:
    // x dimension
    unsigned int m_xDim;
};

// Static scalar C/C++ function wrapping specialization
template<typename T, typename... ARGS >
class Function<T(ARGS...)>
    : public Function<T, sizeof...(ARGS), 1>
{
    static_assert(are_assignable<T &, ARGS...>::value,
                  "StaticScalarFunction arguments must be compatible with return type T");

public:
    // Constructors/Destructor
    Function(std::function<T(ARGS...)> &&fcn)
        : m_Fcn(std::forward<std::function<T(ARGS...)>>(fcn))
    {}
    virtual ~Function() = default;

    // Accessors
    using Function<T, sizeof...(ARGS), 1>::xDim;
    using Function<T, sizeof...(ARGS), 1>::yDim;

    // Evaluators
    virtual T operator() (const T *x) const override
    {
        return call_f(x, typename METAPROG::make_int_seq<0, xDim()>::type());
    }
    using Function<T, sizeof...(ARGS), 1>::operator();

    // Binders
    using Function<T, sizeof...(ARGS), 1>::bind;

private:
    // wrapped std::function
    std::function<T(ARGS...)> m_Fcn;

    template<std::size_t... s>
    T call_f(const T *x, METAPROG::int_seq<s...>) const
    {
        return m_Fcn(std::forward<ARGS>(const_cast < ARGS && > (x[s]))...);
    }

};


/******************************************************************************
 *                           Bind utility functions                           *
 ******************************************************************************/

BEGIN_NAMESPACE(internal)

template<typename F, typename... ARGS>
struct BindHelper;

// Static scalar function bind
template<typename T, unsigned int XDIM>
struct BindHelper<Function<T, XDIM, 1>, const std::vector<T> &>
{
    typedef Function<T()> ReturnType;
    ReturnType bind(Function<T, XDIM, 1> &&f, const std::vector<T> &x)
    {
        return ReturnType(std::bind(static_cast<T(Function<T, XDIM, 1>::*)(const T *) const>(&Function<T, XDIM, 1>::operator()), f, x.data()));
    }
};
template<typename T, unsigned int XDIM>
struct BindHelper<Function<T, XDIM, 1>, const Vector<T> &>
{
    typedef Function<T()> ReturnType;
    ReturnType bind(Function<T, XDIM, 1> &&f, const Vector<T> &x)
    {
        return ReturnType(std::bind(static_cast<T(Function<T, XDIM, 1>::*)(const T *) const>(&Function<T, XDIM, 1>::operator()), f, x.data()));
    }
};
template<typename T, unsigned int XDIM, typename... ARGS>
struct BindHelper<Function<T, XDIM, 1>, ARGS...>
{
    template<typename... Xs>
    static Function<T(Xs...)> dummy(METAPROG::pack<Xs...>);
    typedef decltype(dummy(typename METAPROG::make_pack<T, XDIM>::result())) ReturnType;

    ReturnType bind(Function<T, XDIM, 1> &&f, ARGS &&... args)
    {
        using f_op = T (Function<T, XDIM, 1>::*)(typename if_<(bool)std::is_placeholder<ARGS>::value, T, ARGS>::result...);
        return ReturnType(std::bind(static_cast<f_op>(f), std::forward<ARGS>(args)...));
    }
};

END_NAMESPACE // internal

template<typename F, typename... ARGS>
typename internal::BindHelper<F, ARGS...>::ReturnType bind(F &&f, ARGS &&... args)
{
    return internal::BindHelper<F, ARGS...>::bind(std::forward<F>(f), std::forward<ARGS>(args)...);
}


/******************************************************************************
 *                      Function<T, XDIM, 1> definition                       *
 ******************************************************************************/

template<typename T, unsigned int XDIM>
T Function<T, XDIM, 1>::operator()(const std::vector<T> &x) const
{
    checkXdim(x.size());
    return (*this)(x.data());
}
template<typename T, unsigned int XDIM>
T Function<T, XDIM, 1>::operator()(const Vector<T> &x) const
{
    checkXdim(x.size());
    return (*this)(x.data());
}
template<typename T, unsigned int XDIM>
template<typename... Ts>
T Function<T, XDIM, 1>::operator()(const Ts... xs) const
{
    static_assert(sizeof...(xs) == XDIM, "wrong number of arguments provided");
    static_assert(are_assignable<T &, Ts...>::value,
                  "ScalarFunction provided arguments are not compatible with type T");

    const T x[] = {(T)xs...};
    return (*this)(x);
}

template<typename T, unsigned int XDIM>
Function<T()> Function<T, XDIM, 1>::bind(const std::vector<T> &x) const
{
    return Function<T()>
           (std::bind(static_cast<T(Function<T, XDIM, 1>::*)(const T *) const>(&Function<T, XDIM, 1>::operator()), this, x.data())
            , 0);
}
template<typename T, unsigned int XDIM>
Function<T()> Function<T, XDIM, 1>::bind(const Vector<T> &x) const
{
    return Function<T()>
           (std::bind(static_cast<T(Function<T, XDIM, 1>::*)(const T *) const>(&Function<T, XDIM, 1>::operator()), this, x.data())
            , 0);
}
template<typename T, unsigned int XDIM>
template < typename... Args, typename>
BindReturnTypeHelper<T, METAPROG::pack_count_placeholders<Args...>::value, 1> Function<T, XDIM, 1>::bind(Args... args) const
{
    typedef typename BindReturnTypeHelper<T, METAPROG::pack_count_placeholders<Args...>::value, 1>::type BindReturnType;
    return BindReturnType(std::bind(
                              &Function<T, XDIM, 1>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                              , this, std::forward<Args>(args)...
                          ));
}
template<typename T, unsigned int XDIM>
template<typename BoundF, typename... Ps>
Function<T(Ps...)> Function<T, XDIM, 1>::bind_hlp(BoundF F, METAPROG::pack<Ps...>) const
{
    return Function<T(Ps...)>(F);
}
// template<typename T, unsigned int XDIM>
// template < typename... Args, typename>
// auto Function<T, XDIM, 1>::bind(Args... args) const
// -> decltype(
//     bind_hlp(
//         std::bind(
//             &Function<T, XDIM, 1>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
//             , this, std::forward<Args>(args)...
//         )
//         , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()
//     ))
// {
//     return bind_hlp(
//                std::bind(
//                    &Function<T, XDIM, 1>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
//                    , this, std::forward<Args>(args)...
//                )
//                , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result());
// }

/******************************************************************************
 *                     Function<T, Dynamic, 1> definition                     *
 ******************************************************************************/

template<typename T>
T Function<T, Dynamic, 1>::operator()(const std::vector<T> &x) const
{
    checkXdim(x.size());
    return (*this)(x.data());
}
template<typename T>
T Function<T, Dynamic, 1>::operator()(const Vector<T> &x) const
{
    checkXdim(x.size());
    return (*this)(x.data());
}

template<typename T>
void Function<T, Dynamic, 1>::checkXdim(unsigned int xdim) const
{
    if (m_xDim && xdim != xDim())
    {
        ERROR(SIZE, "wrong number of arguments provided (expected "
              + utils::strFrom(xDim()) + ", got " + utils::strFrom(xdim) + ")");
    }
}


/******************************************************************************
 *                      BindReturnTypeHelper definition                       *
 ******************************************************************************/

template<typename T, unsigned int XDIM>
struct BindReturnTypeHelper<T, XDIM, 1>
{
    template<typename... Xs>
    static Function<T(Xs...)> dummy(METAPROG::pack<Xs...>);
    typedef decltype(dummy(typename METAPROG::make_pack<T, XDIM>::result())) type;
};
template<typename T>
struct BindReturnTypeHelper<T, Dynamic, 1>
{
};
template<typename T, typename... ARGS>
struct BindReturnTypeHelper<T(ARGS...)>
{

};

/******************************************************************************
 *                                  Aliases                                   *
 ******************************************************************************/

template<typename T, int XDIM>
using ScalarFunction = Function<T, XDIM, 1>;




/******************************************************************************
 *                               ScalarFunction                               *
 ******************************************************************************/

// template<typename Signature>
// class StaticScalarFunction;

// template<typename T>
// class ScalarFunction
// {
// private:
//     unsigned int _xDim;

//     template<typename BoundF, typename... Ps>
//     StaticScalarFunction<T(Ps...)> bind_hlp(BoundF F, METAPROG::pack<Ps...>) const;

// public:
//     // Constructors/Destructor
//     explicit ScalarFunction(unsigned int xdim)
//         : _xDim(xdim)
//     {}
//     virtual ~ScalarFunction() = default;

//     // Accessors
//     unsigned int xDim() const
//     {
//         return _xDim;
//     }
//     static unsigned int yDim()
//     {
//         return 1;
//     }

//     // Evaluators
//     virtual T operator()(const T *x) const = 0;
//     T operator()(const std::vector<T> &x) const;
//     T operator()(const Vector<T> &x) const;
//     template<typename... Ts>
//     T operator()(const Ts...x) const;

//     // Bind
//     StaticScalarFunction<T()> bind(const std::vector<T> &x) const;
//     StaticScalarFunction<T()> bind(const Vector<T> &x) const;
//     template < typename... Args,
//                typename = typename std::enable_if <
//                    and_< or_<std::is_assignable<T &, Args>::value, (bool)std::is_placeholder<Args>::value>::value... >::value>::type>
//     auto bind(Args... args) const
//     -> decltype(bind_hlp(std::bind(
//                              &ScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
//                              , this, std::forward<Args>(args)...)
//                          , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()));

// protected:
//     void checkXdim(unsigned int xdim) const;
//     void setXDim(unsigned int xdim)
//     {
//         _xDim = xdim;
//     }
// };

// /******************************************************************************
//  *                            StaticScalarFunction                            *
//  ******************************************************************************/

// template<typename T, typename... Args>
// class StaticScalarFunction<T(Args...)>
//     : public virtual ScalarFunction<T>
// {
//     static_assert(are_assignable<T &, Args...>::value,
//                   "StaticScalarFunction arguments must be compatible with return type T");
// private:
//     // Typedefs
//     typedef std::function<T(Args...)> fcn_type;

// private:
//     fcn_type _Fcn;

// public:
//     StaticScalarFunction(fcn_type f)
//         : ScalarFunction<T>(sizeof...(Args))
//         , _Fcn(f)
//     {}

//     using ScalarFunction<T>::operator();
//     template<typename... Ts>
//     T operator()(const Ts...x) const
//     {
//         static_assert(sizeof...(x) == sizeof...(Args), "wrong number of arguments provided");
//         return _Fcn(x...);
//     }

// protected:
//     virtual T operator() (const T *x) const override
//     {
//         return call_f(x, typename METAPROG::make_int_seq<0, sizeof...(Args)>::type());
//     }

// private:
//     template<std::size_t... s>
//     T call_f(const T *x, METAPROG::int_seq<s...>) const
//     {
//         return _Fcn(std::forward<Args>(const_cast < Args && > (x[s]))...);
//     }
// };



// /******************************************************************************
//  *                         ScalarFunction definition                          *
//  ******************************************************************************/


// template<typename T>
// T ScalarFunction<T>::operator()(const std::vector<T> &x) const
// {
//     checkXdim(x.size());
//     return (*this)(x.data());
// }
// template<typename T>
// T ScalarFunction<T>::operator()(const Vector<T> &x) const
// {
//     checkXdim(x.size());
//     return (*this)(x.data());
// }
// template<typename T>
// template<typename... Ts>
// T ScalarFunction<T>::operator()(const Ts... xs) const
// {
//     static_assert(are_assignable<T &, Ts...>::value,
//                   "ScalarFunction provided arguments are not compatible with type T");
//     // static_assert(are_floating_points<Ts...>::value,
//     //               "ScalarFunction provided arguments are not compatible with type T");

//     const T x[] = {(T)xs...};
//     checkXdim(sizeof...(xs));
//     return (*this)(x);
// }

// template<typename T>
// void ScalarFunction<T>::checkXdim(unsigned int xdim) const
// {
//     if (_xDim && xdim != xDim())
//     {
//         ERROR(SIZE, "wrong number of arguments provided (expected "
//               + utils::strFrom(xDim()) + ", got " + utils::strFrom(xdim) + ")");
//     }
// }

// template<typename T>
// StaticScalarFunction<T()> ScalarFunction<T>::bind(const std::vector<T> &x) const
// {
//     return StaticScalarFunction<T()>
//            (std::bind(static_cast<T(ScalarFunction<T>::*)(const T *) const>(&ScalarFunction<T>::operator()), this, x.data())
//             , 0);
// }

// template<typename T>
// StaticScalarFunction<T()> ScalarFunction<T>::bind(const Vector<T> &x) const
// {
//     return StaticScalarFunction<T()>
//            (std::bind(static_cast<T(ScalarFunction<T>::*)(const T *) const>(&ScalarFunction<T>::operator()), this, x.data())
//             , 0);
// }

// template<typename T >
// template<typename BoundF, typename... Ps>
// StaticScalarFunction<T(Ps...)> ScalarFunction<T>::bind_hlp(BoundF F, METAPROG::pack<Ps...>) const
// {
//     return StaticScalarFunction<T(Ps...)>(F);
// }

// template<typename T>
// template < typename... Args,
//            typename>
// auto ScalarFunction<T>::bind(Args... args) const
// -> decltype(bind_hlp(std::bind(
//                          &ScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
//                          , this, std::forward<Args>(args)...)
//                      , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()))
// {
//     return bind_hlp(
//                std::bind(
//                    &ScalarFunction<T>::operator()< typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result... >
//                    , this, std::forward<Args>(args)...)
//                , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result());
// }


// /******************************************************************************
//  *                               VectorFunction                               *
//  ******************************************************************************/

// // template<typename T>
// // class VectorFunction
// // {
// // private:
// //  unsigned int _xDim, _yDim;

// // public:
// //  // Constructors/Destructor
// //  explicit VectorFunction(unsigned int xdim, unsigned int ydim)
// //  : _xDim(xdim)
// //  , _yDim(ydim)
// //  {}
// //  virtual ~VectorFunction() = default;

// //  // Accessors
// //  unsigned int xDim() const { return _xDim; }
// //  unsigned int yDim() const { return _yDim; }

// //  // Evaluators
// //  virtual T operator()(const T * x) const =0;
// //  T operator()(const std::vector<T>& x) const;
// //  T operator()(const Vector<T>& x) const;
// //  template<typename... Ts>
// //  T operator()(const Ts...x) const;

// // private:
// //  void checkXdim(unsigned int xdim) const;

// // };

END_NAMESPACE // LQCDA

#endif // FUNCTION_HPP