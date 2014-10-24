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

// Generic Scalar function
template<typename T>
class ScalarFunction
{
public: // Constructors/Destructor
    explicit ScalarFunction(const unsigned int xdim = 0);
    virtual ~ScalarFunction() = default;

public: // Queries
    virtual unsigned int xDim() const
    {
        return m_xDim;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }

public: // Evaluators
    virtual T operator()(const T *x) const = 0;
    T operator()(const std::vector<T> &x) const;
    T operator()(const Vector<T> &x) const;
    template<typename... Ts, typename = typename std::enable_if<are_assignable<T &, Ts...>::value>::type>
    T operator()(const Ts...x) const;

protected: // Assignment
    void setXDim(const unsigned int xdim);

private: // Utility functions
    void checkXdim(unsigned int xdim) const;

private: // Data
    unsigned int m_xDim;
};

// Dynamic Scalar function
template<typename T>
class SFunction
    : public ScalarFunction<T>
{
private: // Typedefs
    typedef std::function<T(const T *)> function_type;

public: // Constructors/Destructor
    explicit SFunction(const unsigned int xdim = 0,
                       const function_type &f = nullptr);
    virtual ~SFunction() = default;

public: // Assignment
    void setFunction(const function_type &f, const unsigned int xdim);

public: // Queries
    using ScalarFunction<T>::xDim;
    using ScalarFunction<T>::yDim;

public: // Evaluators
    virtual T operator()(const T *x) const override;
    using ScalarFunction<T>::operator();

private: // Data
    function_type m_f;
};

// Static scalar function
template<typename T, typename... ARGS >
class SFunction<T(ARGS...)>
    : public ScalarFunction<T>
{
    static_assert(are_assignable<T &, ARGS...>::value,
                  "StaticScalarFunction arguments must be compatible with return type T");

private: // Typedefs
    typedef std::function<T(ARGS...)> function_type;

public: // Constructors/Destructor
    SFunction(const function_type &f);
    SFunction(function_type &&fcn)
        : ScalarFunction<T>(sizeof...(ARGS))
        , m_f(std::forward<function_type>(fcn))
    {}
    virtual ~SFunction() = default;

public: // Assignment
    void setFunction(const function_type &f);

public: // Queries
    using ScalarFunction<T>::xDim;
    using ScalarFunction<T>::yDim;

public: // Evaluators
    virtual T operator() (const T *x) const override;
    using ScalarFunction<T>::operator();
    template<typename... Ts, typename = typename std::enable_if<are_assignable<T &, Ts...>::value>::type>
    T operator()(const Ts...x) const;

private: // Utility functions
    template<std::size_t... s>
    T call_f(const T *x, METAPROG::int_seq<s...>) const;

private: // Data
    function_type m_f;
};


/******************************************************************************
 *                        ScalarFunction<T> definition                        *
 ******************************************************************************/

template<typename T>
ScalarFunction<T>::ScalarFunction(const unsigned int xdim)
: m_xDim(xdim)
{}

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
template<typename... Ts, typename>
T ScalarFunction<T>::operator()(const Ts... xs) const
{
    static_assert(are_assignable<T &, Ts...>::value,
                  "ScalarFunction provided arguments are not compatible with type T");
    checkXdim(sizeof...(xs));

    const T x[] = {(T)xs...};
    return (*this)(x);
}

template<typename T>
void ScalarFunction<T>::setXDim(const unsigned int xdim)
{
    m_xDim = xdim;
}

template<typename T>
void ScalarFunction<T>::checkXdim(unsigned int xdim) const
{
    if (m_xDim && xdim != xDim())
    {
        ERROR(SIZE, "wrong number of arguments provided (expected "
              + utils::strFrom(xDim()) + ", got " + utils::strFrom(xdim) + ")");
    }
}

/******************************************************************************
 *                           SFunction<T> definition                          *
 ******************************************************************************/

template<typename T>
SFunction<T>::SFunction(const unsigned int xdim, const function_type &f)
{
    setFunction(f, xdim);
}

template<typename T>
void SFunction<T>::setFunction(const function_type &f, const unsigned int xdim)
{
    ScalarFunction<T>::setXDim(xdim);
    m_f = f;
}

template<typename T>
T SFunction<T>::operator()(const T *x) const
{
    return m_f(x);
}


/******************************************************************************
 *                       SFunction<T(ARGS...)> definition                     *
 ******************************************************************************/

template<typename T, typename... ARGS>
SFunction<T(ARGS...)>::SFunction(const function_type &f)
    : ScalarFunction<T>(sizeof...(ARGS))
{
    setFunction(f);
}

template<typename T, typename... ARGS>
void SFunction<T(ARGS...)>::setFunction(const function_type &f)
{
    m_f = f;
}

template<typename T, typename... ARGS>
T SFunction<T(ARGS...)>::operator()(const T *x) const
{
    return call_f(x, typename METAPROG::make_int_seq<0, sizeof...(ARGS)>::type());
}
template<typename T, typename... ARGS>
template<typename... Ts, typename>
T SFunction<T(ARGS...)>::operator()(const Ts... xs) const
{
    static_assert(are_assignable<T &, Ts...>::value,
                  "ScalarFunction provided arguments are not compatible with type T");
    static_assert(sizeof...(xs) == sizeof...(ARGS), "wrong number of arguments provided");

    return m_f(std::forward<const ARGS>((ARGS)xs)...);
}

template<typename T, typename... ARGS>
template<std::size_t... s>
T SFunction<T(ARGS...)>::call_f(const T *x, METAPROG::int_seq<s...>) const
{
    return m_f(std::forward<ARGS>(const_cast < ARGS && > (x[s]))...);
}


/******************************************************************************
 *                          Any Function Type Erasure                         *
 ******************************************************************************/

// template<typename T>
// class AnyFunction
// {
// public:
//     // Constructors/Destructor
//     AnyFunction()
//         : m_HeldFcn(nullptr)
//     {}
//     template<int XDIM>
//     AnyFunction(const Function<T, XDIM, 1> &fcn)
//         : m_HeldFcn(new FunctionHolder<typename std::decay<const Function<T, XDIM, 1>>::type>(fcn))
//     {}
//     template<typename... ARGS>
//     AnyFunction(const Function<T(ARGS...)> &fcn)
//         : m_HeldFcn(new FunctionHolder<typename std::decay<const Function<T(ARGS...)>>::type>(fcn))
//     {}
//     template<int XDIM>
//     AnyFunction(Function<T, XDIM, 1> &&fcn)
//         : m_HeldFcn(new FunctionHolder<typename std::decay<Function<T, XDIM, 1>>::type>(std::forward < Function<T, XDIM, 1> && > (fcn)))
//     {}

//     AnyFunction(const AnyFunction &other)
//         : m_HeldFcn(other.m_HeldFcn ? other.m_HeldFcn->clone() : 0)
//     {}
//     AnyFunction(AnyFunction &&other)
//         : m_HeldFcn(other.m_HeldFcn)
//     {
//         other.m_HeldFcn = 0;
//     }

//     ~AnyFunction()
//     {
//         delete m_HeldFcn;
//     }

//     // Modifiers
//     AnyFunction &swap(AnyFunction &rhs)
//     {
//         std::swap(m_HeldFcn, rhs.m_HeldFcn);
//         return *this;
//     }
//     AnyFunction &operator=(const AnyFunction &rhs)
//     {
//         AnyFunction(rhs).swap(*this);
//         return *this;
//     }
//     AnyFunction &operator=(AnyFunction && rhs)
//     {
//         rhs.swap(*this);
//         AnyFunction().swap(rhs);
//         return *this;
//     }
//     template<int XDIM>
//     AnyFunction &operator=(Function<T, XDIM, 1> && rhs)
//     {
//         AnyFunction(std::forward < Function<T, XDIM, 1> && > (rhs)).swap(*this);
//         return *this;
//     }

// public:
//     // Queries
//     const std::type_info &type() const
//     {
//         return m_HeldFcn ? m_HeldFcn->type() : typeid(void);
//     }

// private:
//     // Holder classes
//     class HolderBase
//     {
//     public:
//         // Constructors/Destructor
//         virtual ~HolderBase() = default;

//         // Queries
//         virtual const std::type_info &type() const = 0;
//         virtual HolderBase *clone() const = 0;
//     };

//     template<typename FunctionType>
//     class FunctionHolder
//         : public HolderBase
//     {
//     public:
//         // Constructors/Destructor
//         FunctionHolder(const FunctionType &fcn)
//             : held(fcn)
//         {}
//         FunctionHolder(FunctionType &&fcn)
//             : held(std::forward < FunctionType&& > (fcn))
//         {}
//         virtual ~FunctionHolder() = default;

//     public:
//         // Queries
//         virtual const std::type_info &type() const override
//         {
//             return typeid(FunctionType);
//         }
//         virtual HolderBase *clone() const
//         {
//             return new FunctionHolder(held);
//         }

//     public:
//         // Held function
//         FunctionType held;

//     private:
//         // Prevents copy
//         FunctionHolder &operator=(const FunctionHolder &);
//     };

// private:
//     // Casts TODO

// private:
//     HolderBase *m_HeldFcn;
// };

/******************************************************************************
 *                           Bind utility functions                           *
 ******************************************************************************/

BEGIN_NAMESPACE(internal)

template<typename F, typename... ARGS>
struct BindHelper;

// Scalar function bind
template<typename T>
struct BindHelper<SFunction<T>, const std::vector<T> &>
{
    typedef SFunction<T()> ReturnType;
    static ReturnType bind(SFunction<T> &&f, const std::vector<T> &x)
    {
        auto f_caller = [f](const std::vector<T> &x){return f(x.data());};
        auto f_bind = std::bind(f_caller, x);
        return ReturnType(f_bind);
    }
};
template<typename T>
struct BindHelper<SFunction<T>, const Vector<T> &>
{
    typedef SFunction<T()> ReturnType;
    static ReturnType bind(SFunction<T> &&f, const Vector<T> &x)
    {
        auto f_caller = [f](const Vector<T> &x){return f(x.data());};
        auto f_bind = std::bind(f_caller, x);
        return ReturnType(f_bind);
    }
};
template<typename T, typename... ARGS>
struct BindHelper<SFunction<T>, ARGS...>
{
    template<typename... Xs>
    static SFunction<T(Xs...)> dummy(METAPROG::pack<Xs...>);
    typedef decltype(dummy(typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<typename std::decay<ARGS>::type...>::value>::result())) ReturnType;

    static ReturnType bind(SFunction<T> &&f, ARGS &&... args)
    {
        auto f_caller = [f](typename if_<(bool)std::is_placeholder<typename std::decay<ARGS>::type>::value, T, ARGS>::result... fargs){return f(fargs...);};
        auto f_bind = std::bind(f_caller, std::forward<ARGS>(args)...);
        return ReturnType(f_bind);
    }
};

// Static scalar function bind specialization
template<typename T, typename... FARGS, typename... ARGS>
struct BindHelper<SFunction<T(FARGS...)>, ARGS...>
{
    template<typename... Xs>
    static SFunction<T(Xs...)> dummy(METAPROG::pack<Xs...>);
    typedef decltype(dummy(typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<typename std::decay<ARGS>::type...>::value>::result())) ReturnType;

    static ReturnType bind(SFunction<T(FARGS...)> &&f, ARGS &&... args)
    {
        auto f_caller = [f](FARGS... fargs){return f(fargs...);};
        auto f_bind = std::bind(f_caller, std::forward<ARGS>(args)...);
        return ReturnType(f_bind);
    }
};

END_NAMESPACE // internal

template<typename F, typename... ARGS>
typename internal::BindHelper<typename std::decay<F>::type, ARGS...>::ReturnType bind(F &&f, ARGS &&... args)
{
    return internal::BindHelper<typename std::decay<F>::type, ARGS...>::bind(std::forward<typename std::decay<F>::type>(f), std::forward<ARGS>(args)...);
}


/******************************************************************************
 *                                  Aliases                                   *
 ******************************************************************************/


/******************************************************************************
 *                               VectorFunction                               *
 ******************************************************************************/

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