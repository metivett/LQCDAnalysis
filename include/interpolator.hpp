/*
 * interpolator.hpp
 *
 *  Created on: Feb 11, 2013
 *      Author: Thibaut Metivet
 */

#ifndef INTERPOLATOR_HPP_
#define INTERPOLATOR_HPP_

#include <ostream>
#include <map>
#include "io_utils.hpp"

namespace LQCDA {

/**
 * Structure Interpolator
 * Characterizes the interpolator used to build correlators.
 */
class Interpolator
{
public:
	enum InterpolatorType
		{
			SCALAR,
			PSEUDOSCALAR,
			VECTOR,
			AXIAL_VECTOR
		};
	enum SmearingType
	{
		POINT,
		GAUSSIAN
	};
	static std::map<InterpolatorType, std::string> types_table;
	static std::map<SmearingType, std::string> smearings_table;

	Interpolator(InterpolatorType src_type, InterpolatorType snk_type, SmearingType src, SmearingType snk);

	std::string str (LQCDA::IOFormat format) const;
	friend std::ostream& operator<<(std::ostream& out, Interpolator& i);

	inline InterpolatorType getSinkType () const { return sink_type; }
	inline InterpolatorType getSourceType () const { return source_type; }
	inline SmearingType getSinkSmearingType () const { return sink_smearing_type; }
	inline SmearingType getSourceSmearingType () const { return source_smearing_type; }

protected:
	InterpolatorType source_type, sink_type;	// Interpolator type
	SmearingType source_smearing_type, sink_smearing_type;	// Source and sink smearing types
private:
	//Interpolator ();
};

std::ostream& operator<<(std::ostream& out, const Interpolator& i);


} // namespace LQCDA


#endif /* INTERPOLATOR_HPP_ */
