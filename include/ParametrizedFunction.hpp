/*
 * ParametrizedFunction.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PARAMETRIZED_FUNCTION_HPP
#define PARAMETRIZED_FUNCTION_HPP

#include <functional>

#include "Globals.hpp"
#include "Function.hpp"
#include "MetaProgUtils.hpp"
#include "Exceptions.hpp"

BEGIN_NAMESPACE(LQCDA)

/******************************************************************************
 *                           Parametrized Function                            *
 ******************************************************************************/

template<typename T, int XDIM, int YDIM, int NPAR>
class ParametrizedFunction;

// Forward declarations
// Scalar ParametrizedFunction
template<typename T, unsigned int XDIM, unsigned int NPAR>
class ParametrizedFunction<T, XDIM, 1, NPAR>;
template<typename T, unsigned int XDIM>
class ParametrizedFunction<T, XDIM, 1, Dynamic>;
template<typename T, unsigned int NPAR>
class ParametrizedFunction<T, Dynamic, 1, NPAR>;
template<typename T>
class ParametrizedFunction<T, Dynamic, 1, Dynamic>;

// Static scalar parametrized function specialization
template<typename T, unsigned int XDIM, unsigned int NPAR>
class ParametrizedFunction<T, XDIM, 1, NPAR>
{
public:
    // Constructors/Destructor
    ParametrizedFunction() = default;
    virtual ~ParametrizedFunction() = default;

    // Accessors
    static constexpr unsigned int xDim()
    {
        return XDIM;
    }
    static constexpr unsigned int yDim()
    {
        return 1;
    }
    static constexpr unsigned int nPar()
    {
        return NPAR;
    }

    // Evaluators
    virtual T operator()(const T *x, const T *p) const = 0;
    T operator()(const std::vector<T> &x, const std::vector<T> &p) const;
    T operator()(const Vector<T> &x, const Vector<T> &p) const;
    template<typename... Ts>
    T operator()(const Ts...x) const;

    // Binders
    Function<T()> bind(const std::vector<T> &x, const std::vector<T> &p) const;
    Function<T()> bind(const Vector<T> &x, const Vector<T> &p) const;
    Function<T()> bindParameters(const std::vector<T> &p) const;
    Function<T()> bindParameters(const Vector<T> &p) const;
    template < typename... Args,
               typename = typename std::enable_if <are_assignable_or_placeholders<T, Args...>::value>::type>
    auto bind(Args... args) const
    -> decltype(
        bind_hlp(
            std::bind(
                &Function<T, XDIM, 1>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                , this, std::forward<Args>(args)...
            )
            , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()
        ));
};

template<typename T>
class ParametrizedScalarFunction
{
private:
    unsigned int m_xDim;
    unsigned int m_nPar;

    template<typename BoundF, typename... Ps>
    StaticScalarFunction<T(Ps...)> bind_hlp(BoundF F, METAPROG::pack<Ps...>) const;

public:
    // Constructors/Destructor
    explicit ParametrizedScalarFunction(unsigned int xdim, unsigned int npar)
        : m_xDim(xdim)
        , m_nPar(npar)
    {}
    virtual ~ParametrizedScalarFunction() noexcept = default;

    // Accessors
    unsigned int xDim() const
    {
        return m_xDim;
    }
    unsigned int yDim() const
    {
        return 1;
    }
    unsigned int nPar() const
    {
        return m_nPar;
    }

    // Evaluators
    virtual T operator()(const T *x, const T *p) const = 0;
    T operator()(const std::vector<T> &x, const std::vector<T> &p) const;
    T operator()(const Vector<T> &x, const Vector<T> &p) const;
    template<typename... Ts
             >
    T operator()(const Ts...x) const;

    // Bind
    StaticScalarFunction<T()> bind(const std::vector<T> &x, const std::vector<T> &p) const;
    StaticScalarFunction<T()> bind(const Vector<T> &x, const Vector<T> &p) const;
    template < typename... Args,
               typename = typename std::enable_if <
                   and_< or_<std::is_assignable<T &, Args>::value, (bool)std::is_placeholder<Args>::value>::value... >::value>::type>
    auto bind(Args... args) const
    -> decltype(bind_hlp(std::bind(
                             &ParametrizedScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                             , this, std::forward<Args>(args)...)
                         , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()));

private:
    void checkXdim(unsigned int xdim) const;
    void checkParIndex(unsigned int i) const;
    void checkNpar(unsigned int n) const;

};


/******************************************************************************
 *                         StaticParametrizedFunction                         *
 ******************************************************************************/

template<typename T, typename... Args>
class StaticParametrizedScalarFunction<T(Args...)>
    : public virtual ParametrizedScalarFunction<T>
{
    static_assert(are_assignable<T &, Args...>::value,
                  "StaticParametrizedScalarFunction arguments must be compatible with return type T");

private:
    // Typedefs
    typedef std::function<T(Args...)> fcn_type;

    fcn_type m_Fcn;

public:
    // Constructors/Destructor
    StaticParametrizedScalarFunction(fcn_type f, unsigned int npar)
        : ParametrizedScalarFunction<T>(sizeof...(Args) - npar, npar)
        , m_Fcn(f)
    {}
    virtual ~StaticParametrizedScalarFunction() noexcept = default;

    // Accessors
    using ParametrizedScalarFunction<T>::xDim;
    using ParametrizedScalarFunction<T>::yDim;
    using ParametrizedScalarFunction<T>::nPar;

    // Evaluators
    using ParametrizedScalarFunction<T>::operator();

    // Bind
    using ParametrizedScalarFunction<T>::bind;

protected:
    virtual T operator()(const T *x, const T *p) const override
    {
        T *args = new T[sizeof...(Args)];
        std::copy(x, x + xDim(), args);
        std::copy(p, p + nPar(), args + xDim());
        T result = call_f(args, typename METAPROG::make_int_seq<0, sizeof...(Args)>::type());
        delete[] args;
        return result;
    }

private:
    template<std::size_t... s>
    T call_f(const T *args, METAPROG::int_seq<s...>) const
    {
        return m_Fcn(std::forward<Args>(const_cast < Args && > (args[s]))...);
    }

};

/******************************************************************************
 *                      ParametrizedFunction definition                       *
 ******************************************************************************/

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
template<typename... Ts>
T ParametrizedScalarFunction<T>::operator()(const Ts...a) const
{
    static_assert(are_assignable<T &, Ts...>::value,
                  "ParametrizedScalarFunction provided arguments are not compatible with type T");
    const T args[] = {(T)a...};
    this->checkXdim(sizeof...(a) - m_nPar);
    return (*this)(args, args + xDim());
}

template<typename T>
StaticScalarFunction<T()> ParametrizedScalarFunction<T>::bind(const std::vector<T> &x, const std::vector<T> &p) const
{
    return StaticScalarFunction<T()>
           (std::bind(
                static_cast<T(ParametrizedScalarFunction<T>::*)(const T *, const T *) const>(&ParametrizedScalarFunction<T>::operator())
                , this
                , x.data(), p.data()));
}

template<typename T>
StaticScalarFunction<T()> ParametrizedScalarFunction<T>::bind(const Vector<T> &x, const Vector<T> &p) const
{
    return StaticScalarFunction<T()>
           (std::bind(
                static_cast<T(ParametrizedScalarFunction<T>::*)(const T *, const T *) const>(&ParametrizedScalarFunction<T>::operator())
                , this
                , x.data(), p.data()));
}

template<typename T >
template<typename BoundF, typename... Ps>
StaticScalarFunction<T(Ps...)> ParametrizedScalarFunction<T>::bind_hlp(BoundF F, METAPROG::pack<Ps...>) const
{
    return StaticScalarFunction<T(Ps...)>(F);
}

template<typename T>
template < typename... Args,
           typename>
auto ParametrizedScalarFunction<T>::bind(Args... args) const
-> decltype(bind_hlp(std::bind(
                         &ParametrizedScalarFunction<T>::operator()<typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                         , this, std::forward<Args>(args)...)
                     , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result()))
{
    return bind_hlp(
               std::bind(
                   &ParametrizedScalarFunction<T>::operator()< typename if_<(bool)std::is_placeholder<Args>::value, T, Args>::result...>
                   , this, std::forward<Args>(args)...)
               , typename METAPROG::make_pack<T, METAPROG::pack_count_placeholders<Args...>::value>::result());
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


END_NAMESPACE // LQCDA

#endif // PARAMETRIZED_FUNCTION_HPP