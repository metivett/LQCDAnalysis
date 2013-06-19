/*
 * Minuit2Minimizer.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef MINUIT2_MINIMIZER_HPP_
#define MINUIT2_MINIMIZER_HPP_

#include "model_parameters.hpp"
#include "io_utils.hpp"
#include "exceptions.hpp"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnSimplex.h"
#include "Minuit2/MnUserParameters.h"

namespace {
    using namespace ROOT;
    using namespace Minuit2;
}

namespace LQCDA {

    class Mn2Minimizer
    {
    protected:
	static MnUserParameters GetMnUserParameters(const ModelParameters& params);
	static ModelParameters GetModelParameters(const MnUserParameters& params);
    };

    template<class Fcn>
    class Mn2MigradMinimizer : public Mn2Minimizer
    {
    public:
	static ModelParameters Minimize(const Fcn& F, const ModelParameters& params) {
	    MnUserParameters initPar = GetMnUserParameters(params);
	
	    // Pre-minimizer call
	    MnMigrad Migrad1(F, initPar, 0);
	    FunctionMinimum Min = Migrad1();
	
	    LQCDDebug(3)<<"(MINUIT) Pre-minimizer call :\n"
			<< "--------------------------------------------------------"
			<< Min
			<< "--------------------------------------------------------"
			<< std::endl;
	
	    // Minimizer call
	    MnUserParameters pre_min_par = Min.UserParameters();
	    MnMigrad Migrad2(F, pre_min_par, 2);
	    Min = Migrad2();

	    if(!Min.IsValid()) {
		LQCDOut<<"Minuit Library reported that minimization result is not valid !\n";
		throw MinimizationException();
	    }
	    else {
		LQCDDebug(1)<< "(MINUIT) Fit successful !\n"
			    << "Resulting minimum is : "
			    << F
			    << std::endl;
		LQCDDebug(3)<< "(MINUIT) Minimizer call :\n"
			    << "--------------------------------------------------------"
			    << Min
			    << "--------------------------------------------------------"
			    << std::endl;

		return GetModelParameters(Min.UserParameters());
	    }
	}
    };

    template<class Fcn>
    class Mn2SimplexMinimizer : public Mn2Minimizer
    {
    public:
	static ModelParameters Minimize(const Fcn& F, const ModelParameters& params) {
	    MnUserParameters initPar = GetMnUserParameters(params);
	
	    // Pre-minimizer call
	    MnSimplex Migrad1(F, initPar, 0);
	    FunctionMinimum Min = Migrad1();
	
	    LQCDDebug(3)<<"(MINUIT) Pre-minimizer call :\n"
			<< "--------------------------------------------------------"
			<< Min
			<< "--------------------------------------------------------"
			<< std::endl;
	
	    // Minimizer call
	    MnUserParameters pre_min_par = Min.UserParameters();
	    MnSimplex Migrad2(F, pre_min_par, 2);
	    Min = Migrad2();

	    if(!Min.IsValid()) {
		LQCDOut<<"Minuit Library reported that minimization result is not valid !\n";
		throw MinimizationException();
	    }
	    else {
		LQCDDebug(1)<< "(MINUIT) Fit successful !\n"
			    << "Resulting minimum is : "
			    << F
			    << std::endl;
		LQCDDebug(3)<< "(MINUIT) Minimizer call :\n"
			    << "--------------------------------------------------------"
			    << Min
			    << "--------------------------------------------------------"
			    << std::endl;

		return GetModelParameters(Min.UserParameters());
	    }
	}
    };

} // namespace LQCDA

#endif	// MINUIT2_MINIMIZER_HPP_
