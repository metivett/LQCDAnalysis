/*
 * Fitter.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FITTER_HPP_
#define FITTER_HPP_

#include "model_parameters.hpp"
#include "FitResult.hpp"
#include "fit_data.hpp"
#include "models.hpp"
#include <cmath>

namespace LQCDA {

    template<class Fcn, template<class> class Minimizer>
    class Fitter
    {
    private:
	static const double InitError;

	static ModelParameters GetInitialFitParameters(FitDataBase* data, FitModel* model);
	static ModelParameters GetInitialFitParameters(FitDataBase* data, FitModel* model, const std::vector<double>& initParams);
	static ModelParameters GetInitialFitParameters(FitDataBase* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors);

    public:
	static FitResult Fit(FitDataBase* data, FitModel* model);
	static FitResult Fit(FitDataBase* data, FitModel* model, const std::vector<double>& initParams);
	static FitResult Fit(FitDataBase* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors);

    };

    template<class Fcn, template<class> class Minimizer>
    const double Fitter<Fcn, Minimizer>::InitError = 0.1;

// ----------------------------------------------------------------------
// Fitter static Fit member functions
// ----------------------------------------------------------------------
    template<class Fcn, template<class> class Minimizer>
    FitResult Fitter<Fcn, Minimizer>::Fit(FitDataBase* data, FitModel* model)
    {
	Fcn F(data, model);
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	ModelParameters init_par = GetInitialFitParameters(data, model);
	LQCDDebug(2)<<"Initial parameters : "<<'\n'<<init_par;
	
        // Minimize F
	ModelParameters result = Minimizer<Fcn>::Minimize(F, init_par);
	
	return FitResult(data, model, result);
    }

    template<class Fcn, template<class> class Minimizer>
    FitResult Fitter<Fcn, Minimizer>::Fit(FitDataBase* data, FitModel* model, const std::vector<double>& initParams)
    {
	Fcn F(data, model);
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	ModelParameters init_par = GetInitialFitParameters(data, model, initParams);
	LQCDDebug(2)<<"Initial parameters : "<<'\n'<<init_par;
	
        // Minimize F
	ModelParameters result = Minimizer<Fcn>::Minimize(F, init_par);
	
	return FitResult(data, model, result);
    }

    template<class Fcn, template<class> class Minimizer>
    FitResult Fitter<Fcn, Minimizer>::Fit(FitDataBase* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors)
    {
	Fcn F(data, model);
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	ModelParameters init_par = GetInitialFitParameters(data, model, initParams, initErrors);
	LQCDDebug(2)<<"Initial parameters : "<<'\n'<<init_par;
	
        // Minimize F
	ModelParameters result = Minimizer<Fcn>::Minimize(F, init_par);
	
	return FitResult(data, model, result);
    }

// ----------------------------------------------------------------------
// Fitter static GetInitialFitParameters member functions
// ----------------------------------------------------------------------
    template<class Fcn, template<class> class Minimizer>
    ModelParameters Fitter<Fcn, Minimizer>::GetInitialFitParameters(FitDataBase* data, FitModel* model)
    {
	ModelParameters result(model->NbOfParameters());
	if(data->HaveXCorrelation()) {
	    int nxDim = data->nxDim();
	    int nData = data->nData();
// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	    for(int k=0; k<nxDim; ++k) {
		if(data->IsCorrelatedXDim(k)) {
		    for(int i=0; i<nData; ++i) {
// Name for parameter x_ik
			std::ostringstream oss;
			oss<<"x"<<i<<k;
// Add parameter p
			result.Add(oss.str(), data->x(i,k), fabs(data->x(i,k)) * InitError);
		    }
		}
	    }
	}
	return result;
    }
    template<class Fcn, template<class> class Minimizer>
    ModelParameters Fitter<Fcn, Minimizer>::GetInitialFitParameters(FitDataBase* data, FitModel* model, const std::vector<double>& initParams)
    {
	ModelParameters result(initParams);
	if(data->HaveXCorrelation()) {
	    int nxDim = data->nxDim();
	    int nData = data->nData();
// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	    for(int k=0; k<nxDim; ++k) {
		if(data->IsCorrelatedXDim(k)) {
		    for(int i=0; i<nData; ++i) {
// Name for parameter x_ik
			std::ostringstream oss;
			oss<<"x"<<i<<k;
// Add parameter p
			result.Add(oss.str(), data->x(i,k), fabs(data->x(i,k)) * InitError);
		    }
		}
	    }
	}
	return result;
    }
    template<class Fcn, template<class> class Minimizer>
    ModelParameters Fitter<Fcn, Minimizer>::GetInitialFitParameters(FitDataBase* data, FitModel* model, const std::vector<double>& initParams, const std::vector<double>& initErrors)
    {
	ModelParameters result(initParams, initErrors);
	if(data->HaveXCorrelation()) {
	    int nxDim = data->nxDim();
	    int nData = data->nData();
// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	    for(int k=0; k<nxDim; ++k) {
		if(data->IsCorrelatedXDim(k)) {
		    for(int i=0; i<nData; ++i) {
// Name for parameter x_ik
			std::ostringstream oss;
			oss<<"x"<<i<<k;
// Add parameter p
			result.Add(oss.str(), data->x(i,k), fabs(data->x(i,k)) * InitError);
		    }
		}
	    }
	}
	return result;
    }


}

#endif	// FITTER_HPP_
