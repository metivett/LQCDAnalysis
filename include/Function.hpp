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

// Generic scalar function
template<typename T>
class ScalarFunction
{
public:
    // Accessor
    virtual unsigned int xDim() const =0;
    // Evaluator
    virtual T operator()(const T* x) const =0;
};

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
: public ScalarFunction<T>
{
private:
    // Metaprog utility struct for bind arguments
    template<typename U, typename... Args>
    struct are_assignable_or_placeholders
    : and_< or_<std::is_assignable<U &, Args>::value, (bool)std::is_placeholder<Args>::value>::value ... >
    {};

public:
    // Constructors/Destructor
    Function() = default;
    virtual ~Function() = default;

    // Accessors
    virtual unsigned int xDim() const
    {
        return XDIM;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }

    // Evaluators
    using ScalarFunction<T>::operator();
    T operator()(const std::vector<T> &x) const;
    T operator()(const Vector<T> &x) const;
    template<typename... Ts>
    T operator()(const Ts...x) const;

};

// Dynamic scalar function specialization
template<typename T>
class Function<T, Dynamic, 1>
: public ScalarFunction<T>
{
public:
    // Constructors/Destructor
    Function(unsigned int xdim)
        : m_xDim(xdim)
    {}
    virtual ~Function() = default;

    // Accessors
    virtual unsigned int xDim() const
    {
        return m_xDim;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }

    // Evaluators
    using ScalarFunction<T>::operator();
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
    using Function<T, sizeof...(ARGS), 1>::operator();
    virtual T operator() (const T *x) const override
    {
        return call_f(x, typename METAPROG::make_int_seq<0, sizeof...(ARGS)>::type());
    }
    T operator() (const ARGS... args) const
    {
        return m_Fcn(std::forward<const ARGS>(args)...);
    }

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
 *                          Any Function Type Erasure                         *
 ******************************************************************************/

template<typename T>
class AnyFunction
{
public:
    // Constructors/Destructor
    AnyFunction()
    : m_HeldFcn(nullptr)
    {}
    template<int XDIM>
    AnyFunction(const Function<T, XDIM, 1>& fcn)
    : m_HeldFcn(new FunctionHolder<typename std::decay<const Function<T, XDIM, 1>>::type>(fcn))
    {}
    template<typename... ARGS>
    AnyFunction(const Function<T(ARGS...)>& fcn)
    : m_HeldFcn(new FunctionHolder<typename std::decay<const Function<T(ARGS...)>>::type>(fcn))
    {}
    template<int XDIM>
    AnyFunction(Function<T, XDIM, 1>&& fcn)
    : m_HeldFcn(new FunctionHolder<typename std::decay<Function<T, XDIM, 1>>::type>(std::forward<Function<T, XDIM, 1>&&>(fcn)))
    {}

    AnyFunction(const AnyFunction& other)
    : m_HeldFcn(other.m_HeldFcn ? other.m_HeldFcn->clone() : 0)
    {}
    AnyFunction(AnyFunction&& other)
    : m_HeldFcn(other.m_HeldFcn)
    {
        other.m_HeldFcn = 0;
    }

    ~AnyFunction()
    {
        delete m_HeldFcn;
    }

    // Modifiers
    AnyFunction & swap(AnyFunction& rhs)
    {
        std::swap(m_HeldFcn, rhs.m_HeldFcn);
        return *this;
    }
    AnyFunction & operator=(const AnyFunction& rhs)
    {
        AnyFunction(rhs).swap(*this);
        return *this;
    }
    AnyFunction& operator=(AnyFunction&& rhs)
    {
        rhs.swap(*this);
        AnyFunction().swap(rhs);
        return *this;
    }
    template<int XDIM>
    AnyFunction& operator=(Function<T, XDIM, 1>&& rhs)
    {
        AnyFunction(std::forward<Function<T, XDIM, 1>&&>(rhs)).swap(*this);
        return *this;
    }

public:
    // Queries
    const std::type_info & type() const
    {
        return m_HeldFcn ? m_HeldFcn->type(): typeid(void);
    }

private:
    // Holder classes
    class HolderBase
    {
    public:
        // Constructors/Destructor
        virtual ~HolderBase() = default;

        // Queries
        virtual const std::type_info& type() const =0;
        virtual HolderBase * clone() const =0;
    };

    template<typename FunctionType>
    class FunctionHolder
    : public HolderBase
    {
    public:
        // Constructors/Destructor
        FunctionHolder(const FunctionType& fcn)
        : held(fcn)
        {}
        FunctionHolder(FunctionType&& fcn)
        : held(std::forward<FunctionType&&>(fcn))
        {}
        virtual ~FunctionHolder() = default;

    public:
        // Queries
        virtual const std::type_info& type() const override
        {
            return typeid(FunctionType);
        }
        virtual HolderBase * clone() const
        {
            return new FunctionHolder(held);
        }

    public:
        // Held function
        FunctionType held;

    private:
        // Prevents copy
        FunctionHolder & operator=(const FunctionHolder&);
    };

private:
    // Casts TODO

private:
    HolderBase * m_HeldFcn;
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
    static ReturnType bind(Function<T, XDIM, 1> &&f, const std::vector<T> &x)
    {
        using f_op = T(Function<T, XDIM, 1>::*)(const T *) const;
        return ReturnType(std::bind(static_cast<f_op>(Function<T, XDIM, 1>::operator()), f, x.data()));
    }
};
template<typename T, unsigned int XDIM>
struct BindHelper<Function<T, XDIM, 1>, const Vector<T> &>
{
    typedef Function<T()> ReturnType;
    static ReturnType bind(Function<T, XDIM, 1> &&f, const Vector<T> &x)
    {
        using f_op = T(Function<T, XDIM, 1>::*)(const T *) const;
        return ReturnType(std::bind(static_cast<f_op>(Function<T, XDIM, 1>::operator()), f, x.data()));
    }
};
template<typename T, unsigned int XDIM, typename... ARGS>
struct BindHelper<Function<T, XDIM, 1>, ARGS...>
{
    template<typename... Xs>
    static Function<T(Xs...)> dummy(METAPROG::pack<Xs...>);
    typedef decltype(dummy(typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<typename std::decay<ARGS>::type...>::value>::result())) ReturnType;

    static ReturnType bind(Function<T, XDIM, 1> &&f, ARGS &&... args)
    {
        using f_op = T (Function<T, XDIM, 1>::*)(typename if_<(bool)std::is_placeholder<typename std::decay<ARGS>::type>::value, T, ARGS>::result...) const;
        return ReturnType(std::bind(static_cast<f_op>(&Function<T, XDIM, 1>::operator()), f, std::forward<ARGS>(args)...));
    }
};

// Static C/C++ wrapper scalar function bind
template<typename T, typename... FARGS>
struct BindHelper<Function<T(FARGS...)>, const std::vector<T> &>
{
    typedef Function<T()> ReturnType;
    static ReturnType bind(Function<T(FARGS...)> &&f, const std::vector<T> &x)
    {
        using f_op = T(Function<T(FARGS...)>::*)(const T *) const;
        return ReturnType(std::bind(static_cast<f_op>(&Function<T(FARGS...)>::operator()), f, x.data()));
    }
};
template<typename T, typename... FARGS>
struct BindHelper<Function<T(FARGS...)>, const Vector<T> &>
{
    typedef Function<T()> ReturnType;
    static ReturnType bind(Function<T(FARGS...)> &&f, const Vector<T> &x)
    {
        using f_op = T(Function<T(FARGS...)>::*)(const T *) const;
        return ReturnType(std::bind(static_cast<f_op>(&Function<T(FARGS...)>::operator()), f, x.data()));
    }
};
template<typename T, typename... FARGS, typename... ARGS>
struct BindHelper<Function<T(FARGS...)>, ARGS...>
{
    template<typename... Xs>
    static Function<T(Xs...)> dummy(METAPROG::pack<Xs...>);
    typedef decltype(dummy(typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<typename std::decay<ARGS>::type...>::value>::result())) ReturnType;

    static ReturnType bind(Function<T(FARGS...)> &&f, ARGS &&... args)
    {
        using f_op = T (Function<T(FARGS...)>::*)(FARGS...) const;
        return ReturnType(std::bind(static_cast<f_op>(&Function<T(FARGS...)>::operator()), f, std::forward<ARGS>(args)...));
    }
};

END_NAMESPACE // internal

template<typename F, typename... ARGS>
typename internal::BindHelper<typename std::decay<F>::type, ARGS...>::ReturnType bind(F &&f, ARGS &&... args)
{
    return internal::BindHelper<typename std::decay<F>::type, ARGS...>::bind(std::forward<typename std::decay<F>::type>(f), std::forward<ARGS>(args)...);
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
 *                                  Aliases                                   *
 ******************************************************************************/

// template<typename T, int XDIM>
// using ScalarFunction = Function<T, XDIM, 1>;




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