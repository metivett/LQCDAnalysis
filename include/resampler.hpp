/*
 * resampler.hpp
 *
 *  Created on: Apr 09, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RESAMPLER_HPP_
#define RESAMPLER_HPP_

#include "random.hpp"
#include "utils.hpp"
#include "statistics.hpp"
#include <vector>

namespace LQCDA {

/**
 * Resampling policies
 * A resampled sample stores the central value of a statistical quantity computed
 * with a given sample along with its bootstrapped values
 */
    class ResamplerBase
    {
    private:
	unsigned int _NSamples;
	unsigned int _CurrentSample;

    public:
	ResamplerBase(unsigned int nsamples) :
	    _NSamples(nsamples),
	    _CurrentSample(0)
	    {}

	unsigned int NSamples() const { return _NSamples; }
	unsigned int CurrentSample() const { return _CurrentSample; }
	void SetCurrentSample(unsigned int n) {
	    assert(n < _NSamples);
	    _CurrentSample = n;
	}

	// T Mean(const std::vector<T>& sample) const { return DoMean(sample); }
	// T Variance(const std::vector<T>& sample) const { return DoVariance(sample); }
	
    // private:
    // 	virtual T DoMean(const std::vector<T>& sample) const =0;
    // 	virtual T DoVariance(const std::vector<T>& sample) const =0;
	
    };
    
    class BootstrapResampler : public ResamplerBase
    {
    public:
	BootstrapResampler(unsigned int nsamples) :
	    ResamplerBase(nsamples)
	    {}

	template<class U>
	static U Mean(const std::vector<U>& sample) {
	    return LQCDA::mean(sample);
	}
	template<class V>
	static V Variance(const std::vector<V>& sample) {
	    return LQCDA::SampleVariance(sample);
	}
       
    };
    
    template<class T>
    class NoResampling
    {
    public:
	static T mean(const std::vector<T>& sample)
	    {
		if(sample.size() != 1)
		    throw ResamplingException("You're using NoResampling policy with resampled sample!");
		else
		    return sample[0];
	    }
	static T variance(const std::vector<T>& sample)
	    {
		return T(0);
	    }
    };
    /* template<typename T>
    class Resampler
    {
    protected:
	inline virtual std::vector<T> getResampledSet(const std::vector<T>& d, size_t k) const =0;
    public:
	inline virtual T mean(const std::vector<T>& sample) const
	    {
		return LQCDA::mean(sample);
	    }
	inline virtual T variance(const std::vector<T>& sample) const
	    {
		return LQCDA::SampleVariance(sample);
	    }
    };
    
    template<typename T>
    class BootstrapResampler : public Resampler
    {
    protected:
//	RandGen _rng; 		// the random numbers generator
	size_t _nboot;			// number of bootstraps
	size_t _sample_size;		// size of the sample to resample 
	std::vector<std::vector<unsigned int> > _indexes; // resampled indexes

	inline virtual std::vector<T> getResampledSet(const std::vector<T>& d, size_t k) const
	    {
		if(d.size() != _sample_size || k < 0 || k > _nboot)
		    throw OutOfRange("BootstrapResampler", k);
		
		std::vector<T> result(_sample_size);
		for(int i=0; i<_sample_size; ++i) {
		    result[i] = d[_indexes[k][i]];
		}
		return result;
	    }
    public:
	BootstrapResampler(size_t nboot, size_t sample_size)
	    : _nboot(nboot), _sample_size(sample_size), _indexes(nboot) {
	    RandGen rng;
	    for(int k=0; k<nboot; ++k) {
		for(int i=0; i<sample_size; ++i) {
		    _indexes[k].push_back(rng.getUniformInt(sample_size));
		}
	    }
	}
	};*/
    
} // namespace LQCDA

#endif /* RESAMPLER_HPP_ */
