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
#include "Eigen/Dense"
#include "io_utils.hpp"

namespace LQCDA {

    class FitDataBase
    {
    private:
        std::vector<bool> _is_x_corr;

	// Covariance matrices
	Eigen::MatrixXd _C_yy;
	Eigen::MatrixXd _C_xx;
	Eigen::MatrixXd _C_xy;
	
    public:
        FitDataBase(const Eigen::MatrixXd& C_yy,
		    const Eigen::MatrixXd& C_xx,
		    const Eigen::MatrixXd& C_xy,
		    const std::vector<bool>& x_corr =std::vector<bool>())
            : _C_yy(C_yy), _C_xx(C_xx), _C_xy(C_xy),
	      _is_x_corr(x_corr)
        {}

	virtual int nData() const =0;
	virtual int nxDim() const =0;
	virtual int xSize() const =0;
	virtual int nyDim() const =0;
	virtual int ySize() const =0;

	const Eigen::MatrixXd& C_yy() { return _C_yy; }
	const Eigen::MatrixXd& C_xx() { return _C_xx; }
	const Eigen::MatrixXd& C_xy() { return _C_xy; }

	virtual std::vector<double> y(size_t i) const =0;
	virtual double y(size_t i, size_t k) const =0;
	virtual std::vector<double> x(size_t i) const =0;
	virtual double x(size_t i, size_t k) const =0;

	const std::vector<bool>& x_corr() const { return _is_x_corr; }
	
	bool is_x_corr(size_t k) const {
	    if(_is_x_corr.size()==0)
		return false;
	    else
		return _is_x_corr[k];
	}
	bool have_x_corr() const {
	    if(_is_x_corr.size()==0)
		return false;
	    else
		return true;
	}

    protected:
	template <class V>
	static Eigen::MatrixXd compute_C_xx(const std::vector<V>& data, const std::vector<V>& x);
	template <class V>
	static Eigen::MatrixXd compute_C_yy(const std::vector<V>& data);
	template <class V>
	static Eigen::MatrixXd compute_C_xy(const std::vector<V>& data, const std::vector<V>& x);	
    };

    
//-----------------------------------------------------------
// FitData utility functions
//-----------------------------------------------------------
    template <class V>
    Eigen::MatrixXd FitDataBase::compute_C_xx(const std::vector<V>& data, const std::vector<V>& x)
    {
	int nData = data.size();
	int nxDim = x[0].size();
	Eigen::MatrixXd C_xx(nxDim*nData,nxDim*nData);
	for(int i=0; i<nData; ++i) {
	    for(int j=0; j<nData; ++j) {
		C_xx.block(i,j,nxDim,nxDim) = covariance(x[i],x[j]);
	    }
	}

	return C_xx;
    }
    template <class V>
    Eigen::MatrixXd FitDataBase::compute_C_yy(const std::vector<V>& data)
    {
	int nData = data.size();
	int nyDim = data[0].size();
	Eigen::MatrixXd C_yy(nyDim*nData,nyDim*nData);
	for(int i=0; i<nData; ++i) {
	    for(int j=0; j<nData; ++j) {
		C_yy.block(i,j,nyDim,nyDim) = covariance(data[i],data[j]);
	    }
	}
	
	return C_yy;
    }
    template <class V>
    Eigen::MatrixXd FitDataBase::compute_C_xy(const std::vector<V>& data, const std::vector<V>& x)
    {
	int nData = data.size();
	int nxDim = x[0].size();
	int nyDim = data[0].size();
	Eigen::MatrixXd C_xy(nxDim*nData,nyDim*nData);
	for(int i=0; i<nData; ++i) {
	    for(int j=0; j<nData; ++j) {
		C_xy.block(i,j,nxDim,nyDim) = covariance(x[i],data[j]);
	    }
	}

	return C_xy;
    }

    class ResampledFitDataBase : public FitDataBase
    {
    protected:
	unsigned int _nSamples;	// number of samples
	unsigned int _currentSample; // current sample

    public:
	ResampledFitDataBase(unsigned int nSamples,
			     const Eigen::MatrixXd& C_yy,
			     const Eigen::MatrixXd& C_xx,
			     const Eigen::MatrixXd& C_xy,
			     const std::vector<bool>& x_corr =std::vector<bool>())
	    : FitDataBase(C_yy,C_xx,C_xy,x_corr), _nSamples(nSamples)
	    {}

	unsigned int nSample() const { return _nSamples; }
	
	void setCurrentSample(unsigned int k) {
	    if(k >= _nSamples)
		throw ResamplingException("Unexistent sample !");
	    else
		_currentSample = k;
	}
	
    };

    class FitData : public FitDataBase
    {
    private:
	const std::vector<std::vector<double> >& _data;	// data ("y-points") to be fitted
	const std::vector<std::vector<double> >& _x;	// "x-points" ie points s.t. we evalutate model(x,params)
	
    public:
	// TODO
	FitData(const std::vector<std::vector<double> >& data,
		const std::vector<std::vector<double> >& x,
		const Eigen::MatrixXd& C_yy,
		const Eigen::MatrixXd& C_xx,
		const Eigen::MatrixXd& C_xy,
		const std::vector<bool>& x_corr =std::vector<bool>())
	    : _data(data), _x(x),
	      FitDataBase(C_yy, C_xx, C_xy, x_corr)
	    {}
	FitData(const std::vector<std::vector<double> >& data,
		const std::vector<std::vector<double> >& x,
		const std::vector<bool>& x_corr =std::vector<bool>())
	    : _data(data), _x(x),
	      FitDataBase(compute_C_yy(data), compute_C_xx(data, x), compute_C_xy(data, x), x_corr)
	    {}

	// Getters
	int nData() const { return _data.size(); }
	int nxDim() const { return _x[0].size(); }
	int xSize() const { return nData() * nxDim(); }
	int nyDim() const { return _data[0].size(); }
	int ySize() const { return nData() * nyDim(); }
	
	std::vector<double> y(size_t i) const { return (_data)[i]; }
	double y(size_t i, size_t k) const { return (_data)[i][k]; }
	std::vector<double> x(size_t i) const { return (_x)[i]; }
	double x(size_t i, size_t k) const { return (_x)[i][k]; }
    };


    template<template <typename T> class Resampler>
    class ResampledFitData : public ResampledFitDataBase
    {
    private:
	std::vector<RandomVector<Resampler> > _data;	// data ("y-points") to be fitted
	std::vector<RandomVector<Resampler> > _x;	// "x-points" ie points s.t. we evalutate model(x,params)		

    public:
        ResampledFitData(const std::vector<RandomVector<Resampler> >& data,
			 const std::vector<RandomVector<Resampler> >& x,
			 const std::vector<bool>& x_corr =std::vector<bool>())
	    : _data(data), _x(x),
	      ResampledFitDataBase(data[0].nSample(), compute_C_yy(data), compute_C_xx(data, x), compute_C_xy(data, x), x_corr)
	{}

	// Getters
	int nData() const { return _data.size(); }
	int nxDim() const { return _x[0].size(); }
	int xSize() const { return nData() * nxDim(); }
	int nyDim() const { return _data[0].size(); }
	int ySize() const { return nData() * nyDim(); }
	
	std::vector<double> y(size_t i) const { return (_data)[i].vector(_currentSample); }
	double y(size_t i, size_t k) const { return (_data)[i][k].value(_currentSample); }
	std::vector<double> x(size_t i) const { return (_x)[i].vector(_currentSample); }
	double x(size_t i, size_t k) const { return (_x)[i][k].value(_currentSample); }

    };

} // namespace LQCDA


#endif	// FIT_DATA_HPP_
