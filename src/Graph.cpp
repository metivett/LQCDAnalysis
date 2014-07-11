/*
 * Graph.cpp
 *
 *  Created on: May 07, 2014
 *      Author: Thibaut Metivet
 */

 #include "Graph.hpp"

 using namespace LQCDA;
 using namespace PLOT;

 Graph::Axis& Graph::Axis::operator<<(GraphModifier::AxisModifier&& m)
 {
 	m(_Opts);
 	return *this;
 }

 PlotObject& Graph::S(unsigned int i)
 {
 	if(i >= _Objects.size())
 		ERROR(SIZE, "no set " + utils::strFrom(i) + " in the plot");
 	
 	return *_Objects[i];
 }

 const PlotObject& Graph::S(unsigned int i) const
 {
 	if(i >= _Objects.size())
 		ERROR(SIZE, "no set " + utils::strFrom(i) + " in the plot");
 	
 	return *_Objects[i];
 }

 const GraphOptions& Graph::options() const
 {
 	return _Opts;
 }

 Graph& Graph::operator<<(GraphModifier&& m)
 {
 	m(_Opts);
 	return *this;
 }