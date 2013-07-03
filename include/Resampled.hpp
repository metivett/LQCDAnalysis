/*
 * Resampled.hpp
 *
 *  Created on: May 23, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RESAMPLED_HPP_
#define RESAMPLED_HPP_

#include <assert.h>
#include <memory>
#include "resampler.hpp"

namespace LQCDA {

    class IsResampled
    {
    private:
    
    public:    
	virtual ~IsResampled() =0;

	virtual unsigned int NSamples() const =0;
	virtual unsigned int CurrentSample() const =0;
	virtual void SetCurrentSample(unsigned int n) =0;
	
    };

    inline IsResampled::~IsResampled()
    {}

    template<class T,
	     template<class> class Resampler>
    class Resampled
    {
    private:
	std::vector<T> _Sample;

	typedef std::shared_ptr<Resampler<T> > ResamplerPtr;
	ResamplerPtr _Resampler;

    public:
	typedef T resampled_type;

    public:
	Resampled(const std::vector<T>& s) :
	    _Sample(s),
	    _Resampler(std::make_shared<Resampler<T> >(s.size()))
	    {}

	// Accessors
	unsigned int NSamples() const { return _Sample.size(); }
	T& Value(unsigned int n) { return _Sample[n]; }
	const T& Value(unsigned int n) const { return _Sample[n]; }
	const std::vector<T>& Sample() const { return _Sample; }
	T Mean() const { return _Resampler->Mean(_Sample); }
	T Variance() const { return _Resampler->Variance(_Sample); }

	// Operators
        Resampled<T, Resampler>& operator= (const Resampled<T, Resampler>& v);
	Resampled<T, Resampler>& operator= (Resampled<T, Resampler>&& v);
	Resampled<T, Resampler>& operator+= (const Resampled<T, Resampler>& y);
	Resampled<T, Resampler>& operator+= (double d);
	Resampled<T, Resampler>& operator-= (const Resampled<T, Resampler>& y);
	Resampled<T, Resampler>& operator-= (double d);
	Resampled<T, Resampler>& operator*= (const Resampled<T, Resampler>& y);
	Resampled<T, Resampler>& operator*= (double d);
    };

    template<class T,
	     template<class> class Resampler>
    Resampled<T, Resampler>& Resampled<T, Resampler>::operator= (const Resampled<T, Resampler>& v)
    {
	_Sample = v._Sample;
	_Resampler = v._Resampler;
	return *this;
    }

    template<class T,
	     template<class> class Resampler>
    Resampled<T, Resampler>& Resampled<T, Resampler>::operator= (Resampled<T, Resampler>&& v)
    {
	_Sample = std::move(v._Sample);
	_Resampler = std::move(v._Resampler);
	return *this;
    }
    
/*
 * RandomVariable utility functions and operators
 */    
    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler>& Resampled<T, Resampler>::operator+= (const Resampled<T, Resampler>& y) {
	_Sample = _Sample + y._Sample;
	return *this;
    }
    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler>& Resampled<T, Resampler>::operator+= (double d) {
	_Sample = _Sample + d;
	return *this;
    }
    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler>& Resampled<T, Resampler>::operator-= (const Resampled<T, Resampler>& y) {
	_Sample = _Sample - y._Sample;
	return *this;
    }
    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler>& Resampled<T, Resampler>::operator-= (double d) {
	_Sample = _Sample - d;
	return *this;
    }
    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler>& Resampled<T, Resampler>::operator*= (const Resampled<T, Resampler>& y) {
	_Sample = _Sample * y._Sample;
	return *this;
    }
    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler>& Resampled<T, Resampler>::operator*= (double d) {
	_Sample = _Sample * d;
	return *this;
    }

    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler> operator+ (const Resampled<T, Resampler>& x, const Resampled<T, Resampler>& y)
    { 
	return Resampled<T, Resampler>(x.Sample() + y.Sample());
    }

    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler> operator+ (const Resampled<T, Resampler>& x, double d)
    {
	return Resampled<T, Resampler>(x.Sample() + d);
    }

    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler> operator- (const Resampled<T, Resampler>& x, const Resampled<T, Resampler>& y)
    { 
	return Resampled<T, Resampler>(x.Sample() - y.Sample());
    }

    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler> operator- (const Resampled<T, Resampler>& x, double d)
    {
	return Resampled<T, Resampler>(x.Sample() - d);
    }

    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler> operator* (const Resampled<T, Resampler>& x, const Resampled<T, Resampler>& y)
    { 
	return Resampled<T, Resampler>(x.Sample() * y.Sample());
    }

    template<class T,
	     template<class> class Resampler>
    inline Resampled<T, Resampler> operator* (const Resampled<T, Resampler>& x, double d)
    {
	return Resampled<T, Resampler>(x.Sample() * d);
    }
    // TODO
    // template<class T,
    // 	     template<class> class Resampler>
    // double Covariance(const Resampled<T, Resampler>& x, const Resampled<T, Resampler>& y)
    // {
    // 	return SampleCovariance(x.Sample(), y.Sample());
    // }


} // namespace LQCDA

#endif	// RESAMPLED_HPP
