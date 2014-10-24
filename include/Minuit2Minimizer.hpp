/*
 * Minuit2Minimizer.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef MINUIT2_MINIMIZER_HPP_
#define MINUIT2_MINIMIZER_HPP_

#include "Minimizer.hpp"

#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnSimplex.h"
#include "Minuit2/MnUserParameters.h"

#include "Minuit2/MnMinos.h"


BEGIN_NAMESPACE(LQCDA)
BEGIN_NAMESPACE(MIN)

class MnMigradMinimizerOptions
    : public MinimizerOptions
{
public:
    unsigned int level;
    bool pre_minimize;
    unsigned int pre_min_level;
    double error_definition;

public:
    MnMigradMinimizerOptions()
        : MinimizerOptions()
    {
        init();
    }
    MnMigradMinimizerOptions(const MinimizerOptions &opts)
        : MinimizerOptions(opts)
    {
        init();
    }

    virtual ~MnMigradMinimizerOptions() noexcept = default;

    virtual void print(std::ostream &os) const override
    {
        os << "MIGRAD options:\n"
           << "\tlevel = " << level << std::endl
           << "\tpre_minimize = " << pre_minimize << std::endl
           << "\tpre_minimize level = " << pre_min_level << std::endl
           << "\terror_definition = " << error_definition << std::endl;
    }

private:
    void init()
    {
        level = 2;
        pre_minimize = true;
        pre_min_level = 1;
        error_definition = 1.;
    }
};

template<typename T>
class MnMigradMinimizer
    : public Minimizer<T>
{
public:
    // Typedefs
    typedef MnMigradMinimizerOptions OptionsType;
    typedef MIGRAD_ID ID;

private:
    class Mn2FCNWrapper
        : public ROOT::Minuit2::FCNBase
    {
    private:
        const ScalarFunction<T> &_F;
        const MnMigradMinimizerOptions& _Opts;

    public:
        Mn2FCNWrapper(const ScalarFunction<T> &f, const MnMigradMinimizerOptions& opts)
        : FCNBase()
        , _F(f)
        , _Opts(opts)
        {}
        virtual ~Mn2FCNWrapper() {}

        virtual double Up () const
        {
            return _Opts.error_definition;
        }
        virtual double operator()(const std::vector<double> &args) const
        {
            return _F(args);
        }
    };

private:
    OptionsType _Opts;

public:
    // Constructor
    MnMigradMinimizer(const OptionsType &opts = OptionsType())
        : _Opts(opts)
    {}
    // Destructor
    virtual ~MnMigradMinimizer() noexcept = default;

    // Options
    virtual OptionsType &options() override
    {
        return _Opts;
    }

    // Minimize
    using Minimizer<T>::minimize;
    virtual typename Minimizer<T>::Result minimize(
        const ScalarFunction<T> &F,
        const std::vector<T> &x0,
        const std::vector<ScalarConstraint<T>> &c) override;

};

template<typename T>
using MIGRAD = MnMigradMinimizer<T>;

template<typename T>
typename Minimizer<T>::Result MnMigradMinimizer<T>::minimize(
    const ScalarFunction<T> &F,
    const std::vector<T> &x0,
    const std::vector<ScalarConstraint<T>> &c)
{
    utils::vostream vout(std::cout, _Opts.verbosity);
    vout(NORMAL) << "Minimizing with Minuit2 MIGRAD minimizer\n";
    vout(NORMAL) << _Opts << std::endl;
    // LQCDA::MIN::operator<< <T>(vout(NORMAL), _Opts) << std::endl;

    vout(NORMAL) << "Initial parameters:\n";
    std::vector<T> e0(x0.size(), 0.1);
    // for_each(e0.begin(), e0.end(), [](T& x){x*=0.01;});
    ROOT::Minuit2::MnUserParameters params(x0, e0);
    for (int i = 0; i < c.size(); i++)
    {
        if(c[i].hasFixedValue())
        {
            params.Fix(i);
            params.SetValue(i, c[i].fixedValue());
        }
        if (c[i].hasLowerBound())
            params.SetLowerLimit(i, c[i].lowerBound());
        if (c[i].hasUpperBound())
            params.SetUpperLimit(i, c[i].upperBound());
    }
    vout(NORMAL) << params << std::endl;

    auto MnF = new Mn2FCNWrapper(F, _Opts);

    if (_Opts.pre_minimize)
    {
        vout(DEBUG) << "(MINUIT) Pre-minimizer call :\n"
                    << "--------------------------------------------------------";
        ROOT::Minuit2::MnMigrad migrad1(*MnF, params, _Opts.pre_min_level);
        auto preMin = migrad1();
        vout(DEBUG) << preMin
                    << "--------------------------------------------------------"
                    << std::endl;

    }
    vout(DEBUG) << "(MINUIT) Minimizer call :\n"
                << "--------------------------------------------------------";
    ROOT::Minuit2::MnMigrad migrad2(*MnF, params, _Opts.level);
    auto Min =  migrad2();

    delete MnF;

    typename Minimizer<T>::Result result;
    result.final_cost = Min.Fval();
    result.is_valid = Min.IsValid();
    result.minimum = Min.UserParameters().Params();
    result.errors = Min.UserParameters().Errors();

    if (!Min.IsValid())
    {
        vout(NORMAL) << "Minuit Library reported that minimization result is not valid !\n";
        vout(DEBUG)
                << "--------------------------------------------------------"
                << Min
                << "--------------------------------------------------------"
                << std::endl;

        return result;
    }
    else
    {
        vout(NORMAL) << "(MINUIT) Fit successful !\n"
                     << "Resulting minimum is : " << std::endl
                     << "--------------------------------------------------------"
                     << Min
                     << "--------------------------------------------------------"
                     << std::endl;

        return result;
    }
}


// template<typename T>
// void RegisterMnMigradMinimizer()
// {
//  static MinimizerFactoryRegistrar<MnMigradMinimizer, T> MFR;
// }


//   template<class Fcn>
// class Mn2SimplexMinimizer : public Mn2Minimizer
// {
// public:
//  static ModelParameters Minimize(const Fcn& F, const ModelParameters& params) {
//      MnUserParameters initPar = GetMnUserParameters(params);

//    // Pre-minimizer call
//      MnSimplex Migrad1(F, initPar, 2);
//      FunctionMinimum Min = Migrad1();

//      LQCDDebug(3)<<"(MINUIT) Pre-minimizer call :\n"
//      << "--------------------------------------------------------"
//      << Min
//      << "--------------------------------------------------------"
//      << std::endl;

//    // Minimizer call
//      MnUserParameters pre_min_par = Min.UserParameters();
//      MnSimplex Migrad2(F, pre_min_par, 2);
//      Min = Migrad2();

//      if(!Min.IsValid()) {
//          LQCDOut<<"Minuit Library reported that minimization result is not valid !\n";
//          return GetModelParameters(Min.UserParameters());
//      }
//      else {
//          LQCDDebug(1)<< "(MINUIT) Fit successful !\n"
//          << "Resulting minimum is : "
//          << F
//          << std::endl;
//          LQCDDebug(3)<< "(MINUIT) Minimizer call :\n"
//          << "--------------------------------------------------------"
//          << Min
//          << "--------------------------------------------------------"
//          << std::endl;

//          return GetModelParameters(Min.UserParameters());
//      }
//  }
// };

END_NAMESPACE
END_NAMESPACE


#endif  // MINUIT2_MINIMIZER_HPP_
