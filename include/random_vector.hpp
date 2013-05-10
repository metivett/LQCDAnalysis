/*
 * random_vector.hpp
 *
 *  Created on: Apr 09, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RANDOM_VECTOR_HPP_
#define RANDOM_VECTOR_HPP_

#include "random_variable.hpp"
#include "exceptions.hpp"
#include "Eigen/Dense"

namespace LQCDA {

/*
 * class Random Vector
 * A vector of random variables
 */
    template<template <typename T> class Resampler>
    class RandomVector : public Resampler<double>
    {
    protected:
	std::vector<RandomVariable<Resampler> > _m_variables; // random variables
	std::vector<double> _m_mean; // vector of mean values
	Eigen::MatrixXd _m_var;	// correlated variance

	int _nSample;

    public:
	// Constructors
	RandomVector() : _m_variables(), _m_mean(), _m_var() {}
	RandomVector(const std::vector<RandomVariable<Resampler> >& variables);

	// Accessors
	int size() const { return _m_variables.size(); }
	int nSample() const { return _nSample; }
        std::vector<double>  mean() const { return _m_mean; }
	Eigen::MatrixXd var() const { return _m_var; }
	std::vector<double> uncorrelated_var() const;
	const std::vector<RandomVariable<Resampler> >& variables() const { return _m_variables; }
	
	std::vector<double> vector(size_t k) const; // returns a vector made with value number k of each sample

	// Setters
	void addVariable(RandomVariable<Resampler> d);
	void addVariables(const std::vector<RandomVariable<Resampler> >& vals);

	// Operators
	RandomVariable<Resampler>& operator[] (size_t i);
	const RandomVariable<Resampler>& operator[] (size_t i) const;
	std::vector<RandomVariable<Resampler> > getCenteredVector() const;

    private:
	void update(size_t index);
    };

    
/*
 * RandomVector constructor
 */
    template<template <typename T> class Resampler>
    RandomVector<Resampler>::RandomVector(const std::vector<RandomVariable<Resampler> >& variables)
    : _m_variables(variables), _m_mean(variables.size()), _m_var(variables.size(), variables.size())
{
    _nSample = variables[0].nSample();
    int n = variables.size();
    for(int i=0; i<n; ++i)
    {
	if(variables[i].nSample()!=_nSample) {
	    throw DataException("You're dealing with incompatible resampled data!");
	}
	_m_mean[i] = variables[i].mean();
	_m_var(i,i) = ((variables[i] - variables[i].mean()) * (variables[i] - variables[i].mean())).mean();
	for(int j=i+1; j<n; ++j)
	{
	    _m_var(i,j) = _m_var(j,i) = ((variables[i] - variables[i].mean()) * (variables[j] - variables[j].mean())).mean();
	}
    }
}

/*
 * RandomVector utility functions and operators
 */
    template<template <typename T> class Resampler>
    void RandomVector<Resampler>::update(size_t index) {
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

    template<template <typename T> class Resampler>
    std::vector<double> RandomVector<Resampler>::uncorrelated_var() const
    {
	int n = _m_variables.size();
	std::vector<double> result(n);
	for (int i=0; i<n; ++i)
	    result[i] = _m_variables[i].var();
	return result;
    }

    template<template <typename T> class Resampler>
    std::vector<double> RandomVector<Resampler>::vector(size_t k) const
    {
	if(k<0 || k>=_nSample)
	    throw OutOfRange("RandomVector::vector", k);
	std::vector<double> result(size());
	for(int i=0; i<size(); ++i) {
	    result[i] = _m_variables[i].value(k);
	}
	return result;
    }

    template<template <typename T> class Resampler>
    void RandomVector<Resampler>::addVariable(RandomVariable<Resampler> d) {
	size_t curr_ind = _m_variables.size();
	_m_variables.push_back(d);
	update(curr_ind);
    }

    template<template <typename T> class Resampler>
    void RandomVector<Resampler>::addVariables(const std::vector<RandomVariable<Resampler> >& vals) {
	size_t curr_ind = _m_variables.size();
	_m_variables.reserve(curr_ind + vals.size());
	_m_variables.insert(_m_variables.end(), vals.begin(), vals.end());
	update(curr_ind);
    }
    
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVector<Resampler>::operator[] (size_t i) {
	if(i<0 || i>_m_variables.size()-1)
	    throw OutOfRange("RandomVector", i);
	return _m_variables[i];
    }
    template<template <typename T> class Resampler>
    inline const RandomVariable<Resampler>& RandomVector<Resampler>::operator[] (size_t i) const {
	if(i<0 || i>_m_variables.size()-1)
	    throw OutOfRange("RandomVector", i);
	return _m_variables[i];
    }
    template<template <typename T> class Resampler>
    inline std::vector<RandomVariable<Resampler> > RandomVector<Resampler>::getCenteredVector() const {
	std::vector<RandomVariable<Resampler> > result = _m_variables;
	for(int i=0; i<result.size(); ++i) {
	    result[i] -= _m_mean[i];
	}
	return result;
    }

    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator+ (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    { 
	return RandomVector<Resampler>(x.variables() + y.variables());
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator+ (const RandomVector<Resampler>& x, const std::vector<double>& v)
    {
	std::vector<RandomVariable<Resampler> > var = x.variables();
	if(v.size() != var.size())
	    throw DataException("You're adding vectors of different sizes!");
	for(int i=0; i<var.size(); ++i)
	    var[i] += v[i];
	return RandomVector<Resampler>(var);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator+ (const RandomVector<Resampler>& x, double d)
    {
	std::vector<RandomVariable<Resampler> > var = x.variables();
	for(int i=0; i<var.size(); ++i)
	    var[i] += d;
	return RandomVector<Resampler>(var);
    }

    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator- (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    { 
	return RandomVector<Resampler>(x.variables() - y.variables());
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator- (const RandomVector<Resampler>& x, const std::vector<double>& v)
    { 
	std::vector<RandomVariable<Resampler> > var = x.variables();
	if(v.size() != var.size())
	    throw DataException("You're adding vectors of different sizes!");
	for(int i=0; i<var.size(); ++i)
	    var[i] -= v[i];
	return RandomVector<Resampler>(var);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator- (const RandomVector<Resampler>& x, double d)
    { 
	std::vector<RandomVariable<Resampler> > var = x.variables();

	for(int i=0; i<var.size(); ++i)
	    var[i] -= d;
	return RandomVector<Resampler>(var);
    }

    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator* (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    { 
	return RandomVector<Resampler>(x.variables() * y.variables());
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator* (const RandomVector<Resampler>& x, const std::vector<double>& v)
    { 
	std::vector<RandomVariable<Resampler> > var = x.variables();
	if(v.size() != var.size())
	    throw DataException("You're adding vectors of different sizes!");
	for(int i=0; i<var.size(); ++i)
	    var[i] *= v[i];
	return RandomVector<Resampler>(var);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator* (const RandomVector<Resampler>& x, double d)
    { 
	std::vector<RandomVariable<Resampler> > var = x.variables();

	for(int i=0; i<var.size(); ++i)
	    var[i] *= d;
	return RandomVector<Resampler>(var);
    }

    template<template <typename T> class Resampler>
    RandomVariable<Resampler> dotprod (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    {
	if(x.size() == 0 || x.size() != y.size())
	    throw DataException("You're trying to dotproduct random vectors with either null or different sizes !");
	RandomVariable<Resampler> result = x[0] * y[0];
	for(int i=1; i<x.size(); ++i)
	    result += x[i] * y[i];
	return result;
    }

    template<template <typename T> class Resampler>
    Eigen::MatrixXd covariance(const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    {
	if(x.size() == 0 || y.size() == 0)
	    throw DataException("You're trying to compute covariance of random vectors with either null sizes !");

	Eigen::MatrixXd result(x.size(), y.size());
	std::vector<RandomVariable<Resampler> > X_tilda = x.getCenteredVector();
	std::vector<RandomVariable<Resampler> > Y_tilda = y.getCenteredVector();
    
	for(int i=0; i<x.size(); ++i)
	    for(int j=0; j<y.size(); ++j)
		result(i,j) = (X_tilda[i] * Y_tilda[j]).mean();

	return result;
    }
    
} // namespace LQCDA

#endif /* RANDOM_VECTOR_HPP_ */
