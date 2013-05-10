/*
 * random_variable.hpp
 *
 *  Created on: Apr 09, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RANDOM_VARIABLE_HPP_
#define RANDOM_VARIABLE_HPP_

#include <vector>
#include <memory>
#include "resampler.hpp"
#include "utils.hpp"
#include "statistics.hpp"

namespace LQCDA {
       
/*
 * class RandomVariable
 * A random variable, characterized by its sample values
 */
    template<template <typename T> class Resampler>
    class RandomVariable : public Resampler<double>
    {
    private:
	std::vector<double> _m_sample; // resampled values 
	double _m_mean;	// mean value
	double _m_var; // variance

    public:
    // Constructors
        RandomVariable(const std::vector<double>& sample);
        RandomVariable(const RandomVariable<Resampler>& v);
        RandomVariable();

    // Accessors
	double mean() const { return _m_mean; }
	double var() const { return _m_var; }
	const std::vector<double>& values() const { return _m_sample; }
	double value(size_t k) const { return _m_sample[k]; }
	int nSample() const { return _m_sample.size(); }

    // Operators
        RandomVariable<Resampler>& operator= (const RandomVariable<Resampler>& v);
	RandomVariable<Resampler>& operator+= (const RandomVariable<Resampler>& y);
	RandomVariable<Resampler>& operator+= (double d);
	RandomVariable<Resampler>& operator-= (const RandomVariable<Resampler>& y);
	RandomVariable<Resampler>& operator-= (double d);
	RandomVariable<Resampler>& operator*= (const RandomVariable<Resampler>& y);
	RandomVariable<Resampler>& operator*= (double d);

    private:
	void update();
    };

/*
 * RandomVariable constructors!
 */
    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable(const std::vector<double>& sample)
	: _m_sample(sample)
    {
	update();
    }
    
    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable(const RandomVariable<Resampler>& v)
	: _m_sample(v._m_sample), _m_mean(v._m_mean), _m_var(v._m_var)
    {}

    template<template <typename T> class Resampler>
    RandomVariable<Resampler>::RandomVariable()
	: _m_sample(), _m_mean(0), _m_var(0)
    {}

    template<template <typename T> class Resampler>
    RandomVariable<Resampler>& RandomVariable<Resampler>::operator= (const RandomVariable<Resampler>& v)
    {
	_m_sample = v._m_sample;
	_m_mean = v._m_mean;
	_m_var = v._m_var;
	return *this;
    }

/*
 * RandomVariable utility functions and operators
 */
    template<template <typename T> class Resampler>
    void RandomVariable<Resampler>::update()
    {
	_m_mean = Resampler<double>::mean(_m_sample);
	_m_var = Resampler<double>::variance(_m_sample);
    }
    
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator+= (const RandomVariable<Resampler>& y) {
	_m_sample = _m_sample + y.values();
	update();
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator+= (double d) {
	_m_sample = _m_sample + d;
	update();
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator-= (const RandomVariable<Resampler>& y) {
	_m_sample = _m_sample - y.values();
	update();
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator-= (double d) {
	_m_sample = _m_sample - d;
	update();
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator*= (const RandomVariable<Resampler>& y) {
	_m_sample = _m_sample * y.values();
	update();
	return *this;
    }
    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler>& RandomVariable<Resampler>::operator*= (double d) {
	_m_sample = _m_sample * d;
	update();
	return *this;
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator+ (const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    { 
	return RandomVariable<Resampler>(x.values() + y.values());
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator+ (const RandomVariable<Resampler>& x, double d)
    {
	return RandomVariable<Resampler>(x.values() + d);
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator- (const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    { 
	return RandomVariable<Resampler>(x.values() - y.values());
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator- (const RandomVariable<Resampler>& x, double d)
    {
	return RandomVariable<Resampler>(x.values() - d);
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator* (const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    { 
	return RandomVariable<Resampler>(x.values() * y.values());
    }

    template<template <typename T> class Resampler>
    inline RandomVariable<Resampler> operator* (const RandomVariable<Resampler>& x, double d)
    {
	return RandomVariable<Resampler>(x.values() * d);
    }

    template<template <typename T> class Resampler>
    double covariance(const RandomVariable<Resampler>& x, const RandomVariable<Resampler>& y)
    {
	return SampleCovariance(x.values(), y.values());
    }
    
} // namespace LQCDA



#endif /* RANDOM_VARIABLE_HPP_ */
