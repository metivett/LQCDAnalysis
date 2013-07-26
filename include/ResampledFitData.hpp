/*
 * ResampledFitData.hpp
 *
 *  Created on: Jun 19, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RESAMPLED_FIT_DATA_HPP_
#define RESAMPLED_FIT_DATA_HPP_

#include "FitData.hpp"
#include "FitDataWrapper.hpp"
#include "RandomVector.hpp"

namespace LQCDA {

    template<class Resampler,
	     class DataT, class XT,
	     template<class, class> class YYCovariancePolicy,
	     template<class, class> class XXCovariancePolicy,
	     template<class, class> class XYCovariancePolicy>
    class ResampledFitData : public Resampled<FitDataWrapper<DataT, XT>, Resampler>
    {
    private:
	typedef Resampled<FitDataWrapper<DataT, XT>, Resampler> Base;
	typedef XXCovariancePolicy<Resampled<XT, Resampler>, Resampled<XT, Resampler>> XXCov;
	typedef YYCovariancePolicy<Resampled<DataT, Resampler>, Resampled<DataT, Resampler>> YYCov;
	typedef XYCovariancePolicy<Resampled<XT, Resampler>, Resampled<DataT, Resampler>> XYCov;

	std::vector<std::vector<DataT>> _RSData;
	std::vector<std::vector<DataT>> _RSX;

	Eigen::MatrixXd _CovYY;
	Eigen::MatrixXd _CovXX;
	Eigen::MatrixXd _CovXY;

	static std::vector<FitDataWrapper<DataT, XT>> BuildWrappers(const std::vector<std::vector<DataT>>& rsdata,
								    const std::vector<std::vector<XT>>& rsx,
								    Eigen::MatrixXd* cyy,
								    Eigen::MatrixXd* cxx,
								    Eigen::MatrixXd* cxy) {
	    unsigned int nData = rsdata.size();
	    unsigned int nSample = rsdata[0].size();

	    std::vector<FitDataWrapper<DataT, XT>> res;
	    res.reserve(nSample);
	    const DataT* * databuf = new const DataT* [nData];
	    const XT* * xbuf = new const XT* [nData];
	    
	    for(unsigned int n = 0; n < nSample; ++n) {
		for(unsigned int i = 0; i < nData; ++i) {
		    databuf[i] = &rsdata[i][n];
		    xbuf[i] = &rsx[i][n];
		}
		res.push_back(FitDataWrapper<DataT, XT>(databuf, xbuf, nData, cyy, cxx, cxy));
	    }

	    return res;
	}

    public:
	// Construct from data
	// Here data[i] corresponds to the resampled DataT at point i
	// And x[i] to the resampled XT at the same point
	ResampledFitData(const std::vector<std::vector<DataT>>& data,
			 const std::vector<std::vector<XT>>& x) :
	    _RSData(data),
	    _RSX(x),
	    Base(BuildWrappers(data, x, &_CovYY, &_CovXX, &_CovXY))
	    {
		std::vector<Resampled<DataT, Resampler>> datatmp(data.begin(), data.end());
		std::vector<Resampled<XT, Resampler>> xtmp(x.begin(), x.end());
		_CovYY = YYCov::Covariance(datatmp, datatmp);
		_CovXX = XXCov::Covariance(xtmp, xtmp);
		_CovXY = XYCov::Covariance(xtmp, datatmp);
	    }

    };

// template<template <typename T> class Resampler,
// 	 template<class, class> class YYCovariancePolicy,
// 	 template<class, class> class XXCovariancePolicy,
// 	 template<class, class> class XYCovariancePolicy>
// class ResampledFitData : public FitData<RandomVector<Resampler>, RandomVector<Resampler>,
// 					YYCovariancePolicy,
// 					XXCovariancePolicy,
// 					XYCovariancePolicy>,
// 			 public IsResampled
//     {
//     private:
// 	typedef RandomVector<Resampler> RandVec;
// 	typedef FitData<RandVec, RandVec, YYCovariancePolicy, XXCovariancePolicy, XYCovariancePolicy> Base;
//         typedef XXCovariancePolicy<RandVec, RandVec> XXCov;
// 	typedef YYCovariancePolicy<RandVec, RandVec> YYCov;
// 	typedef XYCovariancePolicy<RandVec, RandVec> XYCov;

// 	typedef FitDataWrapper<std::vector<double>, std::vector<double>> Wrapper;

// 	using Base::Reserve;
// 	using Base::Add;

// 	typedef std::shared_ptr<Resampler<double> > ResamplerPtr;
// 	ResamplerPtr _Resampler;

//     public:
// 	ResampledFitData() :
// 	    Base(),
// 	    _Resampler(0)
// 	    {}
//         ResampledFitData(const std::vector<std::vector<std::vector<double> > >& data,
// 			 const std::vector<std::vector<std::vector<double> > >& x) :
// 	    Base(),
// 	    _Resampler(std::make_shared<Resampler<double> >(data[0][0].size())) {
// 		this->Reserve(data.size());
// 		for(int i = 0; i < data.size(); ++i) {
// 		    this->Add(RandVec(data[i], _Resampler), RandVec(x[i], _Resampler));
// 		}
// 	    }

// 	void Add(const std::vector<std::vector<double> >& data,
// 		 const std::vector<std::vector<double> >& x) {
// 	    if(!_Resampler)
// 		_Resampler = std::make_shared<Resampler<double> >(data[0].size());

// 	    Add(RandVec(data, _Resampler), RandVec(x, _Resampler));
// 	}
	
// 	virtual unsigned int NSamples() const { return _Resampler->NSamples(); }
// 	virtual unsigned int CurrentSample() const { return _Resampler->CurrentSample(); }
// 	virtual void SetCurrentSample(unsigned int n) { _Resampler->SetCurrentSample(n); }
// 	Wrapper SampleValue(unsigned int n) const;
// 	Wrapper CurrentValue() const;
	
//     protected:
// 	using Base::Data;
// 	using Base::X;

//     private:
// 	using Base::C_yy;
// 	using Base::C_xx;
// 	using Base::C_xy;
	
//     };

//     namespace {
// 	typedef FitDataWrapper<std::vector<double>, std::vector<double>> Wrapper;
	
// 	template<class RSType>
// 	std::vector<std::vector<double>> SampleValue(const std::vector<RSType>& v, unsigned int n)
// 	{
// 	    std::vector<std::vector<double>> res(v.size());
// 	    for(unsigned int i = 0; i < v.size(); ++v) {
// 		res[i] = v[i].SampleValue(n);
// 	    }

// 	    return res;
// 	}
// 	template<class RSType>
// 	std::vector<std::vector<double>> CurrentValue(const std::vector<RSType>& v, unsigned int n)
// 	{
// 	    std::vector<std::vector<double>> res(v.size());
// 	    for(unsigned int i = 0; i < v.size(); ++v) {
// 		res[i] = v[i].CurrentValue();
// 	    }

// 	    return res;
// 	}
//     }

//     template<template <typename T> class Resampler,
// 	     template<class, class> class YYCovariancePolicy,
// 	     template<class, class> class XXCovariancePolicy,
// 	     template<class, class> class XYCovariancePolicy>
//     Wrapper ResampledFitData<Resampler, YYCovariancePolicy, XXCovariancePolicy, XYCovariancePolicy>::SampleValue(unsigned int n) const
//     {
// 	return Wrapper(SampleValue(Data(), n), SampleValue(X(), n), C_yy(), C_xx(), C_xy());
//     }

//     template<template <typename T> class Resampler,
// 	     template<class, class> class YYCovariancePolicy,
// 	     template<class, class> class XXCovariancePolicy,
// 	     template<class, class> class XYCovariancePolicy>
//     Wrapper ResampledFitData<Resampler, YYCovariancePolicy, XXCovariancePolicy, XYCovariancePolicy>::CurrentValue() const
//     {
// 	return Wrapper(CurrentValue(Data()), CurrentValue(X()), C_yy(), C_xx(), C_xy());
//     }

} // namespace LQCDA

#endif	// RESAMPLED_FIT_DATA_HPP_
