/*
 * fit_utils.hpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_UTILS_HPP_
#define FIT_UTILS_HPP_

#include "Minuit2/MnUserParameters.h"

using namespace ROOT;
using namespace Minuit2;

namespace LQCDA {

/*
 * Utility classes to deal with parameters limits
 */
class LimitBase
    {
    protected:
	double limit;
    public:
	LimitBase(double lim)
	    : limit(lim) {}

	virtual void apply(MnUserParameters& params, size_t p) =0;
    };
    
    class LowerLimit : public LimitBase
    {
    public:
	LowerLimit(double l)
	    : LimitBase(l) {}

	void apply(MnUserParameters& params, size_t p) {
	    params.SetLowerLimit(p, limit);
	}
    };
    class UpperLimit : public LimitBase
    {
    public:
	UpperLimit(double u)
	    : LimitBase(u) {}

	void apply(MnUserParameters& params, size_t p) {
	    params.SetUpperLimit(p, limit);
	}
    };
    
    class Limit : public LowerLimit, public UpperLimit
    {
    public:
	Limit(double l, double u)
	    : LowerLimit(l), UpperLimit(u) {}

	void apply(MnUserParameters& params, size_t p) {
	    params.SetLimits(p, LowerLimit::limit, UpperLimit::limit);
	}
    };

} // namespace LQCDA

#endif	// FIT_UTILS_HPP_
