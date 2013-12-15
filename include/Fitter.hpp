/*
 * Fitter.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FITTER_HPP_
#define FITTER_HPP_

#include "ModelParameters.hpp"
#include "FitResult.hpp"
#include "FitData.hpp"
#include "FitModel.hpp"
#include <cmath>
#include <type_traits>

namespace LQCDA {
    
    namespace {
	template<class DataT, class XT>
	using Data = FitDataA<DataT, XT>;

	template<class DataT, class XT>
	using Model = FitModel<DataT, XT>;

	template<class DataT, class XT>
	using Result = FitResult<DataT, XT>;

	template<bool B, typename T>
	using Enable_if = typename std::enable_if<B,T>::type;
    }

    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    class Fitter
    {
    private:
	static const double InitError;

	template<class DataT, class XT>
	static Enable_if<std::is_floating_point<XT>::value, void> SetDummyParameters(ModelParameters& p, Data<DataT, XT>* data);
	template<class DataT, class XT>
	static Enable_if<!std::is_floating_point<XT>::value, void> SetDummyParameters(ModelParameters& p, Data<DataT, XT>* data);
	
	template<class DataT, class XT>
	static ModelParameters GetInitialFitParameters(Data<DataT, XT>* data, Model<DataT, XT>* model);
	template<class DataT, class XT>
	static ModelParameters GetInitialFitParameters(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams);
	template<class DataT, class XT>
	static ModelParameters GetInitialFitParameters(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams, const std::vector<double>& initErrors);

    public:
	template<class DataT, class XT>
	static Result<DataT, XT> Fit(Data<DataT, XT>* data, Model<DataT, XT>* model);
	template<class DataT, class XT>
	static Result<DataT, XT> Fit(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams);
	template<class DataT, class XT>
	static Result<DataT, XT> Fit(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams, const std::vector<double>& initErrors);

    };

    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    const double Fitter<Fcn, Minimizer>::InitError = 0.1;

// ----------------------------------------------------------------------
// Fitter static Fit member functions
// ----------------------------------------------------------------------
    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    Result<DataT, XT> Fitter<Fcn, Minimizer>::Fit(Data<DataT, XT>* data, Model<DataT, XT>* model)
    {
	Fcn<DataT, XT> F(data, model);
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	ModelParameters init_par = GetInitialFitParameters(data, model);
	LQCDDebug(2)<<"Initial parameters : "<<'\n'<<init_par;
	
        // Minimize F
	ModelParameters result = Minimizer<Fcn<DataT, XT>>::Minimize(F, init_par);
	
	return Result<DataT, XT>(data, model, result);
    }

    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    Result<DataT, XT> Fitter<Fcn, Minimizer>::Fit(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams)
    {
	Fcn<DataT, XT> F(data, model);
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	ModelParameters init_par = GetInitialFitParameters(data, model, initParams);
	LQCDDebug(2)<<"Initial parameters : "<<'\n'<<init_par;
	
        // Minimize F
	ModelParameters result = Minimizer<Fcn<DataT, XT>>::Minimize(F, init_par);
	
	return Result<DataT, XT>(data, model, result);
    }

    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    Result<DataT, XT> Fitter<Fcn, Minimizer>::Fit(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams, const std::vector<double>& initErrors)
    {
	Fcn<DataT, XT> F(data, model);
	LQCDDebug(3)<<"\nMinimization function initialized!\n";

	ModelParameters init_par = GetInitialFitParameters(data, model, initParams, initErrors);
	LQCDDebug(2)<<"Initial parameters : "<<'\n'<<init_par;
	
        // Minimize F
	ModelParameters result = Minimizer<Fcn<DataT, XT>>::Minimize(F, init_par);
	
	return Result<DataT, XT>(data, model, result);
    }

// ----------------------------------------------------------------------
// Fitter static GetInitialFitParameters member functions
// ----------------------------------------------------------------------
    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    Enable_if<std::is_floating_point<XT>::value, void> Fitter<Fcn, Minimizer>::SetDummyParameters(ModelParameters& p, Data<DataT, XT>* data)
    {
	if(data->HaveXCorrelation()) {
	    int nData = data->nData();
// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	    if(data->IsCorrelatedXDim(0)) {
		for(int i=0; i<nData; ++i) {
// Name for parameter x_ik
		    std::ostringstream oss;
		    oss<<"x"<<i;
// Add parameter p
		    p.Add(oss.str(), data->x(i), fabs(data->x(i)) * InitError);
		}
	    }
	}
    }
    
    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    Enable_if<!std::is_floating_point<XT>::value, void> Fitter<Fcn, Minimizer>::SetDummyParameters(ModelParameters& p, Data<DataT, XT>* data)
    {
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
			p.Add(oss.str(), data->x(i)[k], fabs(data->x(i)[k]) * InitError);
		    }
		}
	    }
	}
    }
    
    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    ModelParameters Fitter<Fcn, Minimizer>::GetInitialFitParameters(Data<DataT, XT>* data, Model<DataT, XT>* model)
    {
	ModelParameters result(model->NbOfParameters());
	SetDummyParameters(result, data);
	return result;
    }
    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    ModelParameters Fitter<Fcn, Minimizer>::GetInitialFitParameters(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams)
    {
	ModelParameters result(initParams);
	SetDummyParameters(result, data);
	return result;
    }
    template<template<class, class> class Fcn,
	     template<class> class Minimizer>
    template<class DataT, class XT>
    ModelParameters Fitter<Fcn, Minimizer>::GetInitialFitParameters(Data<DataT, XT>* data, Model<DataT, XT>* model, const std::vector<double>& initParams, const std::vector<double>& initErrors)
    {
	ModelParameters result(initParams, initErrors);
	SetDummyParameters(result, data);
	return result;
    }


}

#endif	// FITTER_HPP_