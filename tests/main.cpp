#include "LQCDA.hpp"

#include <iostream>
#include <vector>
#include <type_traits>

using namespace LQCDA;
using namespace PLOT;
//using namespace std;
//using namespace ROOT::Minuit2;

class Line
: public ParametrizedScalarFunction<double>
{
public:
	Line()
	: ParametrizedScalarFunction<double>(1, 1)
	{}

	virtual double operator()(const double* x, const double* p) const 
	{
		return 2.*(*p)*(*x);
	}
};

class F
: public ScalarFunction<double>
{
public:
	F()
	: ScalarFunction<double>(2)
	{}

private:
	virtual double operator()(const double * x) const override 
	{
		double x2 = x[0]*x[0] + x[1] * x[1];
		return x2 - 2 * x[0] + exp(x[1]) + 10.;
	}
};

// template<typename T>
// std::ostream& operator<<(
// 	std::ostream& os, 
// 	const LQCDA::MIN::Options<LQCDA::MinType::MIGRAD>& opts)
// {
// 	os << "MIGRAD options:\n"
// 	<< "\tlevel = " << opts.level << std::endl
// 	<< "\tpre_minimize = " << opts.pre_minimize << std::endl
// 	<< "\tpre_minimize level = " << opts.pre_min_level << std::endl;
// }

int main() {
	Array<double, Dynamic, 2> xy(4, 2);
	xy << 1., 1., 2., 2., 3., 3., 4., 4.;
	GracePlot plot;
	plot.graph(0) << PlotXY(xy);
	plot.display();
}



