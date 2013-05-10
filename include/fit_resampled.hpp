/*
 * fit_resampled.hpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESAMPLED_HPP_
#define FIT_RESAMPLED_HPP_

#include "fit.hpp"

namespace LQCDA {

    template <class Fcn, template <typename T> class Resampler>
    class ResampledFit : public Fit<Fcn>
    {
    private:
        ResampledFitData<Resampler>* _fitdata;
	FitModel* _model;

    public:
	ResampledFit(ResampledFitData<Resampler>* data, FitModel* model)
	    : _fitdata(data), _model(model)//, _min(data->nSample())
	    {}

    private:
        virtual void fit_impl(const std::vector<double>& initPar, const std::vector<LimitBase*>& parLimits);
    };

    
// ResampledFit::fit() member function
    template<class Fcn>
    void ResampledFit<Fcn>::fit_impl(const std::vector<double>& initPar,
				const std::vector<LimitBase*>& parLimits)
    {
	size_t nParModel = _model->nParams();
	int nxDim = _fitdata->nxDim();
	int nyDim = _fitdata->nyDim();
	int nData = _fitdata->nData();
	MnUserParameters init_par;
	
	if(initPar.size() != nParModel) {
	    throw OutOfRange("Fit::fit()", nParModel);
	}
	
	for(int p=0; p<nParModel; ++p) {
	    // Name for parameter p
	    std::string parName = _model->getParName(p);
	    if(parName == "") {
		std::ostringstream oss;
		oss<<"p"<<p;
		parName = oss.str();
	    }
	    // Add parameter p
	    init_par.Add(parName.c_str(), initPar[p], abs(initPar[p]) * InitError);

	    // Set optional limits on parameters
	    if(parLimits.size() == nParModel) {
		parLimits[p]->apply(init_par, p);
	    }
	    else if(parLimits.size() != 0) {
		throw OutOfRange("Fit::fit()", parLimits.size());
	    }
	}
	
	// Add "pseudo-parameters" to deal with cases of x-correlation in dimension k
	for(int k=0; k<nxDim; ++k) {
	    if(_fitdata->is_x_corr(k)) {
		for(int i=0; i<nData; ++i) {
		    // Name for parameter p
		    std::ostringstream oss;
		    oss<<"x"<<i<<k;
		    // Add parameter p
		    init_par.Add(oss.str().c_str(), _fitdata->x(i,k), abs(_fitdata->x(i,k)) * InitError);
		}
	    }
	}

	LQCDOut<<"\nFit dimensions :\n"
	       <<"nxDim = "<<nxDim<<'\n'
	       <<"nyDim = "<<nyDim<<'\n'
	       <<"nData = "<<nData<<'\n';
	LQCDOut<<"\nX-corr = "<<_fitdata->have_x_corr()<<'\n';
		
	// TODO Extend to support several minimizers

        // Fit each resampled sample
	for(int sample=0; sample<_rsfitdata->nSample(); ++sample) {
	    _rsfitdata->setCurrentSample(sample);
	Fcn F(_rsfitdata, _model);
	LQCDOut<<"\nMinimization function initialized!\n";
	
	// Pre-minimizer call
	MnMigrad Migrad1(F, init_par, 0);
	FunctionMinimum Min = Migrad1();
	LQCDOut<<"(MINUIT) Pre-minimizer call :\n"
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
	    _isComputed = true;
	    _min[sample] = Min;
        }
	LQCDOut<<"(MINUIT) Minimizer call :\n"
	       << "--------------------------------------------------------"
	       << Min
	       << "--------------------------------------------------------"
	       << std::endl;
}

	// Scan around position
	std::vector<std::pair<double,double> > ScanRes;
	MnPlot Plot;

	MnScan Scanner(F, Min.UserParameters(), 2);
	LQCDOut<<"(MINUIT) Scan around last position :\n"
	       << "--------------------------------------------------------"
	       <<std::endl;
	for(int i=0; i<init_par.Params().size(); ++i) {
	    LQCDOut<<"Parameter p"<<i<<std::endl;
	    ScanRes = Scanner.Scan(i);
	    Plot(ScanRes);
	}
	LQCDOut<< "--------------------------------------------------------"
	       << std::endl;
    }

} // namespace LQCDA

#endif	// FIT_RESAMPLED_HPP_
