/*
 * GracePlot.hpp
 *
 *  Created on: May 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef GRACE_PLOT_HPP
#define GRACE_PLOT_HPP

 #include "Plot.hpp"
 #include "PlotRenderer.hpp"

 BEGIN_NAMESPACE(LQCDA)
 BEGIN_NAMESPACE(PLOT)

 class GracePlot
 : public Plot
 {
 public:
 	GracePlot(unsigned int nGraphs = 1)
 	: Plot(new internal::GracePlotRenderer(this), nGraphs)
 	{}
 };

 END_NAMESPACE
 END_NAMESPACE

#endif // GRACE_PLOT_HPP