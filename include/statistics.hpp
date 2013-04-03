/*
 * statistics.hpp
 *
 *  Created on: Feb 23, 2013
 *      Author: Thibaut Metivet
 */

#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include <algorithm>
#include <numeric>
#include <gsl/gsl_cdf.h>
#include "random.hpp"
#include "Eigen/Dense"
#include "utils.hpp"

namespace LQCDA {


/**
 * Resampling policies
 * A resampled sample stores the central value of a statistical quantity computed
 * with a given sample along with its bootstrapped values
 */
    // template class RandomVariable declaration
    template <class ResamplingPolicy = BootstrapResampler<double> >
    class RandomVariable;
    
    template<typename T>
    class Resampler
    {
    protected:
	inline virtual static std::vector<T> getResampledSet(const std::vector<T>& d) =0;
    public:
	template<typename Function>
	inline virtual static RandomVariable<Resampler<double> > resample(const std::vector<T>& d) =0;
	inline virtual static T mean(const std::vector<T>& sample)
	    {
		return mean(sample);
	    }
	inline virtual static T variance(const std::vector<T>& sample)
	    {
		return variance(sample);
	    }
    };
    
    template<typename T>
    class BootstrapResampler : public Resampler<T>
    {
    protected:
	RandGen _rng; 		// the random numbers generator
	int _n;			// number of bootstraps

	inline virtual static std::vector<T> getResampledSet(const std::vector<T>& d)
	    {
		std::vector<T> result;
		unsigned int ri;
		for(int i=0; i<_n; ++i)
		{
		    ri = rng.getUniformInt(n);
		    result[i] = d[ri];
		}
		return result;
	    }
    public:
	BootstrapResampler(int n) : _rng(), _n(n) {}
	// Creates a resampled random variable from "per configuration" data, ie
	// applies Function f(resampled data) for _n resampled data sets
	template<typename Function>
	inline virtual static RandomVariable<BootstrapResampler<double> > resample(const std::vector<T>& d, Function f)
	    {
		std::vector<double> sample;
		for(int i=0; i<_n; ++i)
		{
		    ri = rng.getUniformInt(n);
		    result[i] = d[ri];
		}
		return RandomVariable<BootstrapResampler<double>>(sample);
	    }
    };
    
    // CHECK
    template<typename T, typename Function>
    std::vector<typename Function::return_type> resample(const std::vector<T>& dat,
							 Resampler<T> sampler, unsigned int n_resample, Function f) {
	std::vector<typename Function::return_type> result(n_resample+1);
	result[0] = f(dat);
	for(int i=1; i<n_resample+1; ++i)
	    result[i] = f(sampler.resample(dat));

	return result;
    }

    
/*
 * class RandomVariable
 * A random variable, characterized by its sample values
 */
    template <class ResamplingPolicy>
    class RandomVariable
    {
    protected:
	std::vector<double> _m_sample; // resampled values 
	double _m_mean;	// mean value
	double _m_var; // variance

	void update() {
	    _m_mean = ResamplingPolicy::mean(_m_sample);
	    _m_var = ResamplingPolicy::variance(_m_sample);
	}
    public:
	// Constructors
	RandomVariable(const std::vector<double>& sample)
	    : _m_sample(sample)
			{
			    _m_mean = ResamplingPolicy::mean(_m_sample);
			    _m_var = ResamplingPolicy::variance(_m_sample);
			}

	// Accessors
	double mean() const { return _m_mean; }
	double var() const { return _m_var; }
	std::vector<double> values() const { return _m_sample; }

	// Operators
	RandomVariable<ResamplingPolicy>& operator+= (const RandomVariable<ResamplingPolicy>& y) {
	    _m_sample = _m_sample + y.values();
	    update();
	    return *this;
	}
	RandomVariable<ResamplingPolicy>& operator-= (const RandomVariable<ResamplingPolicy>& y) {
	    _m_sample = _m_sample - y.values();
	    update();
	    return *this;
	}
	RandomVariable<ResamplingPolicy>& operator*= (const RandomVariable<ResamplingPolicy>& y) {
	    _m_sample = _m_sample * y.values();
	    update();
	    return *this;
	}
    };

/*
 * RandomVariable utility functions and operators
 */
	template<class ResamplingPolicy>
	RandomVariable<ResamplingPolicy> operator+ (
	    const RandomVariable<ResamplingPolicy>& x, const RandomVariable<ResamplingPolicy>& y)
	    { 
		return RandomVariable<ResamplingPolicy>(x.values() + y.values());
	    }

	template<class ResamplingPolicy>
	RandomVariable<ResamplingPolicy> operator+ (const RandomVariable<ResamplingPolicy>& x, double d)
	    {
		return RandomVariable<ResamplingPolicy>(x.values() + d);
	    }

	template<class ResamplingPolicy>
	RandomVariable<ResamplingPolicy> operator- (
	    const RandomVariable<ResamplingPolicy>& x, const RandomVariable<ResamplingPolicy>& y)
	    { 
		return RandomVariable<ResamplingPolicy>(x.values() - y.values());
	    }

	template<class ResamplingPolicy>
	RandomVariable<ResamplingPolicy> operator- (const RandomVariable<ResamplingPolicy>& x, double d)
	    { 
		return RandomVariable<ResamplingPolicy>(x.values() - d);
	    }
	template<class ResamplingPolicy>
	RandomVariable<ResamplingPolicy> operator* (
	    const RandomVariable<ResamplingPolicy>& x, const RandomVariable<ResamplingPolicy>& y)
	    { 
		return RandomVariable<ResamplingPolicy>(x.values() * y.values());
	    }
	template<class ResamplingPolicy>
	RandomVariable<ResamplingPolicy> operator* (const RandomVariable<ResamplingPolicy>& x, double d)
	    { 
		return RandomVariable<ResamplingPolicy>(x.values() * d);
	    }

	inline double covariance(const RandomVariable& x, const RandomVariable& y)
	    {
		return LQCDA::covariance(x.values(), y.values());
	    }

/*
 * class Random Vector
 * A vector of random variables
 */
    template <class ResamplingPolicy = BootstrapResampler<double>(1000) >
    class RandomVector
    {
    protected:
	std::vector<RandomVariable<ResamplingPolicy> > _m_variables; // random variables
	std::vector<double> _m_mean; // vector of mean values
	Eigen::MatrixXd _m_var;	// correlated variance

	void update(size_t index) {
	    size_t n = _m_variables.size();
	    _m_mean.resize(n);
	    _m_var.conservativeResize(n,n);
	    for(int i=0; i<index; ++i)
	    {
		for(int j=index; j<n; ++j)
		{
		    _m_var(i,j) = _m_var(j,i) = ((_m_variables[i] - _m_variables[i].mean()) * (_m_variables[j] - _m_variables[j].mean())).mean();
		}
	    }
	    for(int i=index; i<n; ++i)
	    {
		_m_mean[i] = _m_variables[i].mean();
		_m_var(i,i) = ((_m_variables[i] - _m_variables[i].mean()) * (_m_variables[i] - _m_variables[i].mean())).mean();
		for(int j=i+1; j<n; ++j)
		{
		    _m_var(i,j) = _m_var(j,i) = ((_m_variables[i] - _m_variables[i].mean()) * (_m_variables[j] - _m_variables[j].mean())).mean();
		}
	    }
	}

	friend Eigen::MatrixXd covariance(const RandomVector<ResamplingPolicy>&, const RandomVector<ResamplingPolicy>&);
    public:
	// Constructors
	RandomVector() : _m_variables(), _m_mean(), _m_var() {}
	RandomVector(const std::vector<RandomVariable<ResamplingPolicy> >& variables)
	    : _m_variables(variables), _m_mean(variables.size()), _m_var(variables.size(), variables.size())
	    {
		int n = variables.size();
		for(int i=0; i<n; ++i)
		{
		    _m_mean[i] = variables[i].mean();
		    _m_var(i,i) = ((variables[i] - variables[i].mean()) * (variables[i] - variables[i].mean())).mean();
		    for(int j=i+1; j<n; ++j)
		    {
			_m_var(i,j) = _m_var(j,i) = ((variables[i] - variables[i].mean()) * (variables[j] - variables[j].mean())).mean();
		    }
		}
	    }

	// Accessors
	int size() const { return _m_variables.size(); }
        std::vector<double>  mean() const { return _m_mean; }
	Eigen::MatrixXd var() const { return _m_var; }
	std::vector<double> uncorrelated_var() const
	    {
		int n = _m_variables.size();
		std::vector<double> result(n);
		for (int i=0; i<n; ++i)
		    result[i] = _m_variables[i].var();
		return result;
	    }
	std::vector<RandomVariable<ResamplingPolicy> > variables() const { return _m_variables; }

	// Setters
	void addVariable(RandomVariable<ResamplingPolicy> d) {
	    size_t curr_ind = _m_variables.size();
	    _m_variables.push_back(d);
	    update(curr_ind);
	}
	void addVariables(const std::vector<RandomVariable<ResamplingPolicy> >& vals) {
	    size_t curr_ind = _m_variables.size();
	    _m_variables.reserve(_m_sample.size() + vals.size());
	    _m_variables.insert(_m_sample.end(), vals.begin(), vals.end());
	    update(curr_ind);
	}

	// Operators
	RandomVariable<ResamplingPolicy>& operator[] (size_t i) {
	    if(i<0 || i>_m_variables.size()-1)
		throw OutOfRange("RandomVector", i);
	    return _m_variables[i];
	}
    };

/*
 * RandomVector utility functions and operators
 */
	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator+ (
	    const RandomVector<ResamplingPolicy>& x, const RandomVector<ResamplingPolicy>& y)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() + y.variables());
	    }

	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator+ (
	    const RandomVector<ResamplingPolicy>& x, const std::vector<double>& v)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() + v);
	    }

	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator+ (const RandomVector<ResamplingPolicy>& x, double d)
	    {
		return RandomVector<ResamplingPolicy>(x.variables() + d);
	    }

	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator- (
	    const RandomVector<ResamplingPolicy>& x, const RandomVector<ResamplingPolicy>& y)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() - y.variables());
	    }

	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator- (
	    const RandomVector<ResamplingPolicy>& x, const std::vector<double>& v)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() - v);
	    }

	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator- (const RandomVector<ResamplingPolicy>& x, double d)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() - d);
	    }
	
	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator* (
	    const RandomVector<ResamplingPolicy>& x, const RandomVector<ResamplingPolicy>& y)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() * y.variables());
	    }

	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator* (
	    const RandomVector<ResamplingPolicy>& x, const std::vector<double>& v)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() * v);
	    }
	
	template<class ResamplingPolicy>
	RandomVector<ResamplingPolicy> operator* (const RandomVector<ResamplingPolicy>& x, double d)
	    { 
		return RandomVector<ResamplingPolicy>(x.variables() * d);
	    }

	template<class ResamplingPolicy>
	inline RandomVariable<ResamplingPolicy> dotprod (const RandomVector<ResamplingPolicy>& x, const RandomVector<ResamplingPolicy>& y)
	    {
		if(x.size() == 0 || x.size() != y.size())
		    throw DataException("You're trying to dotproduct random vectors with either null or different sizes !");
		RandomVariable<ResamplingPolicy> result = x[0] * y[0];
		for(int i=1; i<x.size(); ++i)
		    result += x[i] * y[i];
		return result;
	    }

	template<ResamplingPolicy>
	Eigen::MatrixXd covariance(const RandomVector<ResamplingPolicy>& x, const RandomVector<ResamplingPolicy>& y)
	    {
		if(x.size() == 0 || x.size() != y.size())
		    throw DataException("You're trying to compute covariance of random vectors with either null or different sizes !");
		std::vector<RandomVariable<ResamplingPolicy> > X_tilda = x.variables() - x.mean();
		std::vector<RandomVariable<ResamplingPolicy> > Y_tilda = y.variables() - y.mean();
		Eigen::Matrix<RandomVariable<ResamplingPolicy>, Eigen::Dynamic, 1>::Map(&X_tilda[0], X_tilda.size())
		    * Eigen::Matrix<RandomVariable<ResamplingPolicy>, 1, Eigen::Dynamic>::Map(&Y_tilda[0], Y_tilda.size());
	    }


/*
 * Chi2 p-value given chi2_val value
 * ie returns integral(p(x)dx, x=chi2_val..infinity) with p chi2 distribution
 */
    double chi2_pvalue(const double chi2_val, const size_t ndof)
    {
	return gsl_cdf_chisq_Q(chi2_val,(double)ndof);
    }


}	// namespace LQCDA


#endif /* STATISTICS_HPP_ */
