/*
 * random_vector.hpp
 *
 *  Created on: Apr 09, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RANDOM_VECTOR_HPP_
#define RANDOM_VECTOR_HPP_

#include <assert.h>
#include <memory>
#include "random_variable.hpp"
#include "resampled.hpp"
#include "Eigen/Dense"
#include "io_utils.hpp"

namespace LQCDA {

    template<template <typename T> class Resampler>
    class ResampledFitData; 
/*
 * class Random Vector
 * A vector of random variables
 */
    template<template <typename T> class Resampler>
    class RandomVector : public IsResampled
    {
    private:
	typedef RandomVariable<Resampler> RandVar;
	std::vector<RandVar> _RandomVariables; // random variables

	typedef std::shared_ptr<Resampler<double> > ResamplerPtr;
	ResamplerPtr _Resampler;

	friend class ResampledFitData<Resampler>;
	
	RandomVector(const std::vector<std::vector<double> >& variables, const ResamplerPtr& resampler);
    public:
	// Constructors
	RandomVector(const std::vector<std::vector<double> >& variables);
	RandomVector(const RandomVector<Resampler>& v);
	RandomVector(RandomVector<Resampler>&& v);

	// Destructor
	~RandomVector();

	// Accessors
	int size() const { return _RandomVariables.size(); }
	virtual unsigned int NSamples() const { return _Resampler->NSamples(); }
	virtual unsigned int CurrentSample() const { return _Resampler->CurrentSample(); }
	
	virtual void SetCurrentSample(unsigned int n) { _Resampler->SetCurrentSample(n); }
        std::vector<double> Mean() const;
	std::vector<double> Variance() const;
	const std::vector<RandVar>& RandomVariables() const { return _RandomVariables; }

	// Setters
	void AddVariable(const std::vector<double>& v);

	// Operators
	RandomVector<Resampler>& operator= (const RandomVector<Resampler>& v);
	RandomVector<Resampler>& operator= (RandomVector<Resampler>&& v);
	RandomVariable<Resampler>& operator[] (size_t i);
	const RandomVariable<Resampler>& operator[] (size_t i) const;

	explicit operator std::vector<double>() const;

    };

    
/*
 * RandomVector constructor
 */
    template<template <typename T> class Resampler>
    RandomVector<Resampler>::RandomVector(const std::vector<std::vector<double> >& variables) :
	_RandomVariables(),
	_Resampler(std::make_shared<Resampler<double> >(variables[0].size()))
{
    _RandomVariables.reserve(variables.size());
    for(int i = 0; i < variables.size(); ++i) {
	_RandomVariables.push_back(RandVar(variables[i], _Resampler));
    }
}
    template<template <typename T> class Resampler>
    RandomVector<Resampler>::RandomVector(const std::vector<std::vector<double> >& variables, const ResamplerPtr& resampler) :
	_RandomVariables(),
	_Resampler(resampler)
{
    _RandomVariables.reserve(variables.size());
    for(int i = 0; i < variables.size(); ++i) {
	_RandomVariables.push_back(RandVar(variables[i], _Resampler));
    }
}
    template<template <typename T> class Resampler>
    RandomVector<Resampler>::RandomVector(const RandomVector<Resampler>& v) :
	_RandomVariables(v._RandomVariables),
	_Resampler(v._Resampler)
    {}

    template<template <typename T> class Resampler>
    RandomVector<Resampler>::RandomVector(RandomVector<Resampler>&& v) :
	_RandomVariables(std::move(v._RandomVariables)),
	_Resampler(std::move(v._Resampler))
    {}

    template<template <typename T> class Resampler>
    RandomVector<Resampler>& RandomVector<Resampler>::operator= (const RandomVector<Resampler>& v)
    {
	_RandomVariables = v._RandomVariables;
	_Resampler = v._Resampler;
	return *this;
    }

    template<template <typename T> class Resampler>
    RandomVector<Resampler>& RandomVector<Resampler>::operator= (RandomVector<Resampler>&& v)
    {
	_RandomVariables = std::move(v._RandomVariables);
	_Resampler = std::move(v._Resampler);
	return *this;
    }

/*
 * RandomVector destructor
 */
    template<template <typename T> class Resampler>
    RandomVector<Resampler>::~RandomVector()
    {}
    
/*
 * RandomVector utility functions and operators
 */
    template<template <typename T> class Resampler>
    std::vector<double> RandomVector<Resampler>::Mean() const
    {
	int n = _RandomVariables.size();
	std::vector<double> result(n);
	for (int i=0; i<n; ++i)
	    result[i] = _RandomVariables[i].Mean();
	return result;
    }
    template<template <typename T> class Resampler>
    std::vector<double> RandomVector<Resampler>::Variance() const
    {
	int n = _RandomVariables.size();
	std::vector<double> result(n);
	for (int i=0; i<n; ++i)
	    result[i] = _RandomVariables[i].Variance();
	return result;
    }

    template<template <typename T> class Resampler>
    RandomVector<Resampler>::operator std::vector<double>() const
    {
	std::vector<double> result(size());
	for(int i = 0; i < size(); ++i) {
	    result[i] = static_cast<double>(_RandomVariables[i]);
	}
	return result;
    }

    template<template <typename T> class Resampler>
    void RandomVector<Resampler>::AddVariable(const std::vector<double>& v) {
	_RandomVariables.push_back(RandVar(v, _Resampler));
    }
    
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVector<Resampler>::operator[] (size_t i) {
	return _RandomVariables[i];
    }
    template<template <typename T> class Resampler>
    inline const RandomVariable<Resampler>& RandomVector<Resampler>::operator[] (size_t i) const {
	return _RandomVariables[i];
    }

    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator+ (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    {
	assert(x.size() == y.size());
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() + y[i].Sample();
	}
	return RandomVector<Resampler>(result);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator+ (const RandomVector<Resampler>& x, const std::vector<double>& y)
    {
	assert(x.size() == y.size());
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() + y[i];
	}
	return RandomVector<Resampler>(result);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator+ (const RandomVector<Resampler>& x, double d)
    {
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() + d;
	}
	return RandomVector<Resampler>(result);
    }

    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator- (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    { 
	assert(x.size() == y.size());
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() - y[i].Sample();
	}
	return RandomVector<Resampler>(result);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator- (const RandomVector<Resampler>& x, const std::vector<double>& y)
    { 
	assert(x.size() == y.size());
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() - y[i];
	}
	return RandomVector<Resampler>(result);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator- (const RandomVector<Resampler>& x, double d)
    { 
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() - d;
	}
	return RandomVector<Resampler>(result);
    }

    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator* (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    { 
	assert(x.size() == y.size());
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() * y[i].Sample();
	}
	return RandomVector<Resampler>(result);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator* (const RandomVector<Resampler>& x, const std::vector<double>& y)
    { 
	assert(x.size() == y.size());
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() * y[i];
	}
	return RandomVector<Resampler>(result);
    }
    template<template <typename T> class Resampler>
    inline RandomVector<Resampler> operator* (const RandomVector<Resampler>& x, double d)
    { 
	std::vector<std::vector<double> > result(x.size());
	for(int i = 0; i < x.size(); ++i) {
	    result[i] = x[i].Sample() * d;
	}
	return RandomVector<Resampler>(result);
    }

    template<template <typename T> class Resampler>
    RandomVariable<Resampler> Dotprod (const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    {
	assert(x.size() == y.size());
	RandomVariable<Resampler> result = x[0] * y[0];
	for(int i = 1; i < x.size(); ++i)
	    result += x[i] * y[i];
	return result;
    }

    template<template <typename T> class Resampler>
    Eigen::MatrixXd Covariance(const RandomVector<Resampler>& x, const RandomVector<Resampler>& y)
    {
	Eigen::MatrixXd result(x.size(), y.size());
	std::vector<RandomVariable<Resampler> > X_tilda = (x - x.Mean()).RandomVariables();
	std::vector<RandomVariable<Resampler> > Y_tilda = (y - y.Mean()).RandomVariables();
    
	for(int i=0; i<x.size(); ++i)
	    for(int j=0; j<y.size(); ++j)
		result(i,j) = (X_tilda[i] * Y_tilda[j]).Mean();
	return result;
    }
    
} // namespace LQCDA

#endif /* RANDOM_VECTOR_HPP_ */
