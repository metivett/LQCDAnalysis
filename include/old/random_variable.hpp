/*
 * random_variable.hpp
 *
 *  Created on: Apr 09, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RANDOM_VARIABLE_HPP_
#define RANDOM_VARIABLE_HPP_

#include <vector>
#include <assert.h>
#include <memory>
#include "resampler.hpp"
#include "resampled.hpp"
#include "random_vector.hpp"
#include "utils.hpp"
#include "statistics.hpp"

namespace LQCDA {
    
    template<template <typename T> class Resampler>
    class RandomVector; 
/*
 * class RandomVariable
 * A random variable, characterized by its sample values
 */
    template<template <typename T> class Resampler>
    class RandomVariable : public IsResampled
    {
    private:
	std::vector<double> _Sample; // resampled values 

	typedef std::shared_ptr<Resampler<double> > ResamplerPtr;
	ResamplerPtr _Resampler;

	friend class RandomVector<Resampler>;

	RandomVariable(const std::vector<double>& sample, const ResamplerPtr& resampler);

    public:
    // Constructors
        RandomVariable(const std::vector<double>& sample);
        RandomVariable(const RandomVariable<Resampler>& v);
	RandomVariable(RandomVariable<Resampler>&& v);

    // Destructor
	~RandomVariable();

    // Accessors
	double Mean() const { return _Resampler->Mean(_Sample); }
	double Variance() const { return _Resampler->Variance(_Sample); }
	const std::vector<double>& Sample() const { return _Sample; }
	double SampleValue(size_t k) const { return _Sample[k]; }
	virtual unsigned int NSamples() const { return _Resampler->NSamples(); }
	virtual unsigned int CurrentSample() const { return _Resampler->CurrentSample(); }
	
	virtual void SetCurrentSample(unsigned int n) { _Resampler->SetCurrentSample(n); }

    // Operators
        RandomVariable<Resampler>& operator= (const RandomVariable<Resampler>& v);
	RandomVariable<Resampler>& operator= (RandomVariable<Resampler>&& v);
	RandomVariable<Resampler>& operator+= (const RandomVariable<Resampler>& y);
	RandomVariable<Resampler>& operator+= (double d);
	RandomVariable<Resampler>& operator-= (const RandomVariable<Resampler>& y);
	RandomVariable<Resampler>& operator-= (double d);
	RandomVariable<Resampler>& operator*= (const RandomVariable<Resampler>& y);
	RandomVariable<Resampler>& operator*= (double d);

	explicit operator double() const { return _Sample[_Resampler->CurrentSample()]; }

    };

/*
 * RandomVariable constructors
 */
    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable(const std::vector<double>& sample, const ResamplerPtr& resampler) :
	_Sample(sample),
	_Resampler(resampler)
    {
	assert(sample.size() == resampler->NSamples());
    }
    
    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable(const std::vector<double>& sample) :
	_Sample(sample),
	_Resampler(std::make_shared<Resampler<double> >(sample.size()))
    {}
    
    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable(const RandomVariable<Resampler>& v) :
	_Sample(v._Sample),
	_Resampler(v._Resampler)
    {}

    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable(RandomVariable<Resampler>&& v) :
	_Sample(std::move(v._Sample)),
	_Resampler(std::move(v._Resampler))
    {}

    template<template <typename T> class Resampler>
    RandomVariable<Resampler>& RandomVariable<Resampler>::operator= (const RandomVariable<Resampler>& v)
    {
	_Sample = v._Sample;
	_Resampler = v._Resampler;
	return *this;
    }

    template<template <typename T> class Resampler>
    RandomVariable<Resampler>& RandomVariable<Resampler>::operator= (RandomVariable<Resampler>&& v)
    {
	_Sample = std::move(v._Sample);
	_Resampler = std::move(v._Resampler);
	return *this;
    }

/*
 * RandomVariable destructor
 */
    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::~RandomVariable()
    {}

    
/*
 * RandomVariable utility functions and operators
 */    
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator+= (const RandomVariable<Resampler>& y) {
	_Sample = _Sample + y._Sample;
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator+= (double d) {
	_Sample = _Sample + d;
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator-= (const RandomVariable<Resampler>& y) {
	_Sample = _Sample - y._Sample;
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator-= (double d) {
	_Sample = _Sample - d;
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator*= (const RandomVariable<Resampler>& y) {
	_Sample = _Sample * y._Sample;
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator*= (double d) {
	_Sample = _Sample * d;
	return *this;
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator+ (const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    { 
	return RandomVariable<Resampler>(x.Sample() + y.Sample());
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator+ (const RandomVariable<Resampler>& x, double d)
    {
	return RandomVariable<Resampler>(x.Sample() + d);
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator- (const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    { 
	return RandomVariable<Resampler>(x.Sample() - y.Sample());
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator- (const RandomVariable<Resampler>& x, double d)
    {
	return RandomVariable<Resampler>(x.Sample() - d);
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator* (const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    { 
	return RandomVariable<Resampler>(x.Sample() * y.Sample());
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator* (const RandomVariable<Resampler>& x, double d)
    {
	return RandomVariable<Resampler>(x.Sample() * d);
    }

    template<template <typename T> class Resampler>
    double Covariance(const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    {
	return SampleCovariance(x.Sample(), y.Sample());
    }
    
} // namespace LQCDA



#endif /* RANDOM_VARIABLE_HPP_ */
