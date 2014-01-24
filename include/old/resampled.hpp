/*
 * resampled.hpp
 *
 *  Created on: May 23, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RESAMPLED_HPP_
#define RESAMPLED_HPP_

#include <assert.h>
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

} // namespace LQCDA

#endif	// RESAMPLED_HPP
