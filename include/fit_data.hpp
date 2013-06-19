/*
 * fit_data.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_DATA_HPP_
#define FIT_DATA_HPP_

#include "random_vector.hpp"
#include "statistics.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include "Eigen/Dense"
#include "io_utils.hpp"

namespace LQCDA {

    class FitDataBase
    {
    private:	
        std::vector<bool> _IsCorrelatedXDim;

	/*// Covariance matrices
	Eigen::MatrixXd _C_yy;
	Eigen::MatrixXd _C_xx;
	Eigen::MatrixXd _C_xy;*/
	
    public:
        /*FitDataBase(const Eigen::MatrixXd& C_yy,
		    const Eigen::MatrixXd& C_xx,
		    const Eigen::MatrixXd& C_xy,
		    const std::vector<bool>& x_corr =std::vector<bool>()) :
	    _C_yy(C_yy), _C_xx(C_xx), _C_xy(C_xy),
	    IsCorrelatedXDim(x_corr),
	    _EnabledPointsIndexes
	    {}*/
	FitDataBase(const std::vector<bool>& x_corr) :
	    _IsCorrelatedXDim(x_corr)
	    {}

	virtual int nData() const =0;
	virtual int nxDim() const =0;
	virtual int xSize() const =0;
	virtual int nyDim() const =0;
	virtual int ySize() const =0;

	virtual Eigen::MatrixXd C_yy(unsigned int i, unsigned int j) =0;
	virtual Eigen::MatrixXd C_xx(unsigned int i, unsigned int j) =0;
	virtual Eigen::MatrixXd C_xy(unsigned int i, unsigned int j) =0;

	virtual std::vector<double> y(size_t i) const =0;
	virtual double y(size_t i, size_t k) const =0;
	virtual std::vector<double> x(size_t i) const =0;
	virtual double x(size_t i, size_t k) const =0;

	virtual void DisablePoint(unsigned int i) =0;
	virtual void EnablePoint(unsigned int i) =0;
	virtual void EnableAllPoints() =0;
	
	bool IsCorrelatedXDim(size_t k) const {
	    return _IsCorrelatedXDim[k];
	}
	bool HaveXCorrelation() const {
	    return std::find(_IsCorrelatedXDim.begin(), _IsCorrelatedXDim.end(), true) != _IsCorrelatedXDim.end();
	}
	void SetXDimCorrelation(size_t k, bool b) {
	    _IsCorrelatedXDim[k] = b;
	}

	template <class V>
	static Eigen::MatrixXd ComputeCxx(const std::vector<V>& data, const std::vector<V>& x);
	template <class V>
	static Eigen::MatrixXd ComputeCyy(const std::vector<V>& data);
	template <class V>
	static Eigen::MatrixXd ComputeCxy(const std::vector<V>& data, const std::vector<V>& x);	
    };

    
//-----------------------------------------------------------
// FitDataBase utility functions
//-----------------------------------------------------------
    template <class V>
    Eigen::MatrixXd FitDataBase::ComputeCxx(const std::vector<V>& data, const std::vector<V>& x)
    {
	int nData = data.size();
	int nxDim = x[0].size();
	Eigen::MatrixXd C_xx(nxDim*nData,nxDim*nData);
	for(int i=0; i<nData; ++i) {
	    for(int j=0; j<nData; ++j) {
		C_xx.block(i,j,nxDim,nxDim) = Covariance(x[i],x[j]);
	    }
	}

	return C_xx;
    }
    template <class V>
    Eigen::MatrixXd FitDataBase::ComputeCyy(const std::vector<V>& data)
    {
	int nData = data.size();
	int nyDim = data[0].size();
	Eigen::MatrixXd C_yy(nyDim*nData,nyDim*nData);
	for(int i=0; i<nData; ++i) {
	    for(int j=0; j<nData; ++j) {
		C_yy.block(i,j,nyDim,nyDim) = Covariance(data[i],data[j]);
	    }
	}
	
	return C_yy;
    }
    template <class V>
    Eigen::MatrixXd FitDataBase::ComputeCxy(const std::vector<V>& data, const std::vector<V>& x)
    {
	int nData = data.size();
	int nxDim = x[0].size();
	int nyDim = data[0].size();
	Eigen::MatrixXd C_xy(nxDim*nData,nyDim*nData);
	for(int i=0; i<nData; ++i) {
	    for(int j=0; j<nData; ++j) {
		C_xy.block(i,j,nxDim,nyDim) = Covariance(x[i],data[j]);
	    }
	}

	return C_xy;
    }

    template<class DataT, class XT>
    class FitDataBaseT : public FitDataBase
    {
    private:
	std::vector<DataT> _Data;	// data ("y-points") to be fitted
	std::vector<XT> _X;	// "x-points" ie points s.t. we evalutate model(x,params)
	
	std::vector<unsigned int> _EnabledPointsIndexes;

	Eigen::MatrixXd _CovYY;
	Eigen::MatrixXd _CovXX;
	Eigen::MatrixXd _CovXY;
	bool _UpToDateCov;

    public:
	FitDataBaseT(const std::vector<bool>& x_corr) :
	    FitDataBase(x_corr),
	    _UpToDateCov(true)
	    {}
	FitDataBaseT(const std::vector<DataT>& data,
		     const std::vector<XT>& x,
		     const std::vector<bool>& x_corr) :
	    FitDataBase(x_corr),
	    _Data(data), _X(x),
	    _EnabledPointsIndexes(data.size()),
	    _CovYY(FitDataBase::ComputeCyy(data)),
	    _CovXX(FitDataBase::ComputeCxx(data, x)),
	    _CovXY(FitDataBase::ComputeCxy(data, x)),
	    _UpToDateCov(true)
	    {
		for(int i = 0; i < data.size(); ++i)
		    _EnabledPointsIndexes[i] = i;
	    }
	FitDataBaseT(const std::vector<DataT>& data,
		     const std::vector<XT>& x) :
	    FitDataBaseT(data, x, std::vector<bool>(x[0].size(), false))
	    {}

	// Getters
	int nData() const { return _EnabledPointsIndexes.size(); }
	int nxDim() const { return _X[0].size(); }
	int xSize() const { return nData() * nxDim(); }
	int nyDim() const { return _Data[0].size(); }
	int ySize() const { return nData() * nyDim(); }

	std::vector<double> y(size_t i) const { return static_cast<std::vector<double> >(_Data[_EnabledPointsIndexes[i]]); }
	double y(size_t i, size_t k) const { return static_cast<double>(_Data[_EnabledPointsIndexes[i]][k]); }
	std::vector<double> x(size_t i) const { return static_cast<std::vector<double> >(_X[_EnabledPointsIndexes[i]]); }
	double x(size_t i, size_t k) const { return static_cast<double>(_X[_EnabledPointsIndexes[i]][k]); }

	void Add(const DataT& data, const XT& x) {
	    _Data.push_back(data);
	    _X.push_back(x);
	    _EnabledPointsIndexes.push_back(_Data.size() - 1);
	    _UpToDateCov = false;
	}

	void Reserve(size_t n) {
	    _Data.reserve(n);
	    _X.reserve(n);
	    _EnabledPointsIndexes.reserve(n);
	}
	
	virtual void DisablePoint(unsigned int i) {
	    assert(i < _Data.size());
	    _EnabledPointsIndexes.erase(std::remove(_EnabledPointsIndexes.begin(), _EnabledPointsIndexes.end(), i), _EnabledPointsIndexes.end());
	}
	virtual void EnablePoint(unsigned int i) {
	    assert(i < _Data.size());
	    _EnabledPointsIndexes.insert(std::find_if(_EnabledPointsIndexes.begin(), _EnabledPointsIndexes.end(), std::bind2nd(std::less<int>(), i)), i);
	}
	virtual void EnableAllPoints() {
	    _EnabledPointsIndexes.resize(_Data.size());
	    for(int i = 0; i < _Data.size(); ++i)
		_EnabledPointsIndexes[i] = i;
	}

	// Eigen::MatrixXd C_yy() const {
	//     Eigen::MatrixXd C_yy(nyDim()*nData(),nyDim()*nData());
	//     for(int i=0; i<nData(); ++i) {
	// 	for(int j=0; j<nData(); ++j) {
	// 	    C_yy.block(i * nyDim(), j * nyDim(), nyDim(), nyDim()) = Covariance(_Data[_EnabledPointsIndexes[i]],_Data[_EnabledPointsIndexes[j]]);
	// 	}
	//     }
	
	//     return C_yy;
	// }
	Eigen::MatrixXd C_yy(unsigned int i, unsigned int j) {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovYY.block(_EnabledPointsIndexes[i]*nyDim(), _EnabledPointsIndexes[j]*nyDim(), nyDim(), nyDim());
	}
	// Eigen::MatrixXd C_xx() const {
	//     Eigen::MatrixXd C_xx(nxDim()*nData(),nxDim()*nData());
	//     for(int i=0; i<nData(); ++i) {
	// 	for(int j=0; j<nData(); ++j) {
	// 	    C_xx.block(i * nxDim(), j * nxDim(), nxDim(), nxDim()) = Covariance(_X[_EnabledPointsIndexes[i]],_X[_EnabledPointsIndexes[j]]);
	// 	}
	//     }

	//     return C_xx;
	// }
	Eigen::MatrixXd C_xx(unsigned int i, unsigned int j) {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovXX.block(_EnabledPointsIndexes[i]*nxDim(), _EnabledPointsIndexes[j]*nxDim(), nxDim(), nxDim());
	}
	// Eigen::MatrixXd C_xy() const {
	//     Eigen::MatrixXd C_xy(nxDim()*nData(),nyDim()*nData());
	//     for(int i=0; i<nData(); ++i) {
	// 	for(int j=0; j<nData(); ++j) {
	// 	    C_xy.block(i * nxDim(), j * nyDim(), nxDim(), nyDim()) = Covariance(_X[_EnabledPointsIndexes[i]],_Data[_EnabledPointsIndexes[j]]);
	// 	}
	//     }

	//     return C_xy;
	// }
	Eigen::MatrixXd C_xy(unsigned int i, unsigned int j) {
	    if(!_UpToDateCov) {
		UpdateCovariance();
	    }
	    return _CovXY.block(_EnabledPointsIndexes[i]*nxDim(), _EnabledPointsIndexes[j]*nyDim(), nxDim(), nyDim());
	}

    private:
	void UpdateCovariance() {
	    _CovYY = FitDataBase::ComputeCyy(_Data);
	    _CovXX = FitDataBase::ComputeCxx(_Data, _X);
	    _CovXY = FitDataBase::ComputeCxy(_Data, _X);
	    _UpToDateCov = true;
	}
    };
    

    class FitData : public FitDataBaseT<std::vector<double>, std::vector<double> >
    {
    private:
	typedef FitDataBaseT<std::vector<double>, std::vector<double> > Base;
	using Base::Add;
    public:
	FitData(const std::vector<bool>& x_corr) :
	    Base(x_corr)
	    {}
	FitData(const std::vector<std::vector<double> >& data,
		const std::vector<std::vector<double> >& x,
		const std::vector<bool>& x_corr) :
	    Base(data, x, x_corr)
	    {}
	FitData(const std::vector<std::vector<double> >& data,
		const std::vector<std::vector<double> >& x) :
	    Base(data, x)
	    {}

	void Add(const std::vector<bool>& data, const std::vector<bool>& x) {
	    Add(data, x);
	}
    };


    template<template <typename T> class Resampler>
    class ResampledFitData : public FitDataBaseT<RandomVector<Resampler>, RandomVector<Resampler> >,
			     public IsResampled
    {
    private:
	typedef RandomVector<Resampler> RandVec;
	typedef FitDataBaseT<RandVec, RandVec> Base;
	using Base::Add;

	typedef std::shared_ptr<Resampler<double> > ResamplerPtr;
	ResamplerPtr _Resampler;

    public:
	ResampledFitData(const std::vector<bool>& x_corr) :
	    Base(x_corr),
	    _Resampler(0)
	    {}
        ResampledFitData(const std::vector<std::vector<std::vector<double> > >& data,
			 const std::vector<std::vector<std::vector<double> > >& x,
			 const std::vector<bool>& x_corr) :
	    Base(x_corr),
	    _Resampler(std::make_shared<Resampler<double> >(data[0][0].size()))
	    {
		this->Reserve(data.size());
		for(int i = 0; i < data.size(); ++i) {
		    this->Add(RandVec(data[i], _Resampler), RandVec(x[i], _Resampler));
		}
	    }
	ResampledFitData(const std::vector<std::vector<std::vector<double> > >& data,
			 const std::vector<std::vector<std::vector<double> > >& x) :
	    Base(std::vector<bool>(x[0].size(), false)),
	    _Resampler(std::make_shared<Resampler<double> >(data[0][0].size()))
	    {
		this->Reserve(data.size());
		for(int i = 0; i < data.size(); ++i) {
		    this->Add(RandVec(data[i], _Resampler), RandVec(x[i], _Resampler));
		}
	    }

	void Add(const std::vector<std::vector<double> >& data,
		 const std::vector<std::vector<double> >& x) {
	    if(!_Resampler)
		_Resampler = std::make_shared<Resampler<double> >(data[0].size());

	    Add(RandVec(data, _Resampler), RandVec(x, _Resampler));
	}
	
	virtual unsigned int NSamples() const { return _Resampler->NSamples(); }
	virtual unsigned int CurrentSample() const { return _Resampler->CurrentSample(); }
	virtual void SetCurrentSample(unsigned int n) { _Resampler->SetCurrentSample(n); }
	
    };

} // namespace LQCDA


#endif	// FIT_DATA_HPP_
