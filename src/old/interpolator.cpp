/*
 * interpolator.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: Thibaut Metivet
 */

#include "interpolator.hpp"
#include <sstream>

using namespace LQCDA;

std::map<Interpolator::InterpolatorType, std::string> Interpolator::types_table =
{
		{InterpolatorType::SCALAR, "S"},
		{InterpolatorType::PSEUDOSCALAR, "P"},
		{InterpolatorType::VECTOR, "V3"}
};

std::map<Interpolator::SmearingType, std::string> Interpolator::smearings_table =
{
		{SmearingType::POINT, "P"},
		{SmearingType::GAUSSIAN, "G"}
};

std::string Interpolator::str (IOFormat format) const
{
	std::ostringstream oss;
	switch (format)
	{
	case IOFormat::LATAN_1:
		oss << (smearings_table[sink_smearing_type]);
		oss << (smearings_table[source_smearing_type]);
		oss << (types_table[sink_type]);
		oss << (types_table[source_type]);
		// TODO: Adapt to actual format (this was done to do quick tests)
		oss << (int)0 << (int)0 << (int)-1;
		break;
	}
	return oss.str();
}

Interpolator::Interpolator(InterpolatorType src_type, InterpolatorType snk_type, SmearingType src_smear, SmearingType snk_smear)
: source_type(src_type), sink_type(snk_type), source_smearing_type(src_smear), sink_smearing_type(snk_smear)
{}

std::ostream& operator<<(std::ostream& out, const Interpolator& i)
{
	out << "Sink type : " << Interpolator::types_table[i.getSinkType()] << ", ";
	out << "Source type : " << Interpolator::types_table[i.getSourceType()] << '\n';
	out << "Smearing : ";
	out << "sink " << Interpolator::smearings_table[i.getSinkSmearingType()] << ", ";
	out << "source " << Interpolator::smearings_table[i.getSourceSmearingType()];
	return out;
}

