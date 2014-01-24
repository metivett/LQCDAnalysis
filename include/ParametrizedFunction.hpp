/*
 * ParametrizedFunction.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef PARAMETRIZED_FUNCTION_HPP
#define PARAMETRIZED_FUNCTION_HPP

 #include "Function.hpp"
 #include <array>

 namespace LQCDA {

    // template<typename XT, typename YT, unsigned int NPAR>
    // class ParametrizedFunction: public virtual Function<XT, YT>
    // {
    // private:
    //     INHERIT_FUNCTION_TYPEDEFS;

    //     std::vector<double> _Parameters;

    // public:
    //     ParametrizedFunction(): _Parameters(NPAR) {}
    //     ParametrizedFunction(std::initializer_list<double> p): _Parameters{p} {}

    //     constexpr unsigned int nParams() const { return NPAR; }

    //     const std::vector<double>& getParamValues() const { return _Parameters; }
    //     double getParamValue(unsigned int i) const { assert(i < nParams()); return _Parameters[i]; }

    //     void setParamValues(const std::vector<double>& p) { assert(p.size() == nParams()); _Parameters = p; }
    //     void setParamValue(unsigned int i, double p) { assert(i < nParams()); _Parameters[i] = p; }

    //     y_type operator() (x_arg_type x, const std::vector<double>& p) const {
    //         assert(p.size() == nParams());
    //         return Eval(x, p);
    //     }

    //     virtual y_type operator() (x_arg_type x) const { return this->eval(x, _Parameters); }

    // protected:
    //     virtual y_type eval(x_arg_type x, const std::vector<double>& p) const =0;
    // };

    // template<typename T, int XDIM, int YDIM, unsigned int NPAR>
    // using VParametrizedFunction = ParametrizedFunction<Vector<T, XDIM>, Vector<T, YDIM>, NPAR>;


/******************************************************************************
 *                           Parametrized Function                            *
 ******************************************************************************/

    template<typename Scalar, int XDIM, int YDIM, int NPAR>
    class ParametrizedFunction;

    template<typename Scalar, int XDIM, int YDIM, int NPAR>
    class ParametrizedFunction
    : public virtual Function<Scalar, XDIM, YDIM>
    {
    INHERIT_FUNCTION_TYPEDEFS;
    public:
        // Typedefs
        typedef double par_type;
        typedef array<par_type, NPAR> p_type;

    protected:
        // Typedefs
        typedef typename if_<std::is_fundamental<p_type>::value, p_type, const p_type&>::result params_type;

    private:
        // Parameters
        p_type _P;

    public:
        // Constructors/Destructor
        explicit ParametrizedFunction(): _P() {}
        explicit ParametrizedFunction(params_type p): _P(p) {}

        virtual ~ParametrizedFunction() {}

        // Accessors
        constexpr unsigned int nPar() const { return NPAR; }
        par_type getParValue(unsigned int i) const { assert(i < nPar()); return _P[i]; }
        params_type getParValues() const { return _P; }

        // Setters
        void setParValue(unsigned int i, par_type p) { assert(i < nPar()); _P[i] = p; }
        void setParValues(params_type p) { _P = p; }

        // Evaluators
        y_type operator()(arg_type x, params_type p) { return evaluate(x, p); }

    private:
        virtual y_type evaluate(arg_type x) override { return evaluate(x, _P); }
        virtual y_type evaluate(arg_type x, params_type p) =0;

    };

    template<typename Scalar, int XDIM, int YDIM>
    class ParametrizedFunction<Scalar, XDIM, YDIM, Dynamic>
    : public virtual Function<Scalar, XDIM, YDIM>
    {
    INHERIT_FUNCTION_TYPEDEFS;
    public:
        // Typedefs
        typedef double par_type;
        typedef vector<par_type> p_type;

    protected:
        // Typedefs
        typedef typename if_<std::is_fundamental<p_type>::value, p_type, const p_type&>::result params_type;

    private:
        // Parameters
        unsigned int _nPar;
        p_type _P;

    public:
        // Constructors/Destructor
        explicit ParametrizedFunction(unsigned int npar): _nPar(npar), _P(npar) {}
        explicit ParametrizedFunction(params_type p): _nPar(p.size()), _P(p) {}

        virtual ~ParametrizedFunction() {}

        // Accessors
        constexpr unsigned int nPar() const { return _nPar; }
        par_type getParValue(unsigned int i) const { assert(i < nPar()); return _P[i]; }
        params_type getParValues() const { return _P; }

        // Setters
        void setParValue(unsigned int i, par_type p) { assert(i < nPar()); _P[i] = p; }
        void setParValues(params_type p) { _P = p; }

        // Evaluators
        y_type operator()(arg_type x, params_type p) { return evaluate(x, p); }

    private:
        virtual y_type evaluate(arg_type x) override { return evaluate(x, _P); }
        virtual y_type evaluate(arg_type x, params_type p) =0;

    };

}

#endif // PARAMETRIZED_FUNCTION_HPP