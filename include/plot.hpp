/*
 * plot.hpp
 *
 *  Created on: Mar 5, 2013
 *      Author: Thibaut Metivet
 */

#ifndef PLOT_HPP_
#define PLOT_HPP_

#include <mgl2/mgl.h>
#include "Eigen/Dense"


namespace LQCDA {

    template<typename Scalar>
    mglData mgl(const std::vector<Scalar>& v)
    {
	int n = v.size();
	mglData result(n);
	for(int i=0; i<n; ++i)
	    result.a[i] = v[i];
	return result;
    }

    template<typename Scalar>
    mglData mgl(const Eigen::Array<Scalar, Eigen::Dynamic, 1>& d)
    {
	return mglData(d.size(), d.data());
    }
/*
    class Plotter
    {
    private:
	mglData _Data;
	mglGraph * _Graph;
	
    public:
	Plotter();
	
	void Plot() { DoPlot(); }

	void AddPoint(double x, double y);
	void AddPoint(double x, double y, double xerr, double yerr);

	template<class Fcn>
	void AddCurve(Fcn f);

    private:
	virtual void DoPlot() =0;
	};*/

}	// namespace LQCDA


#endif /* PLOT_HPP_ */
