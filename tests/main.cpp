#include "Fit.hpp"
#include "Minuit2Minimizer.hpp"
#include <iostream>
#include <type_traits>

using namespace LQCDA;
using namespace std;
using namespace ROOT::Minuit2;

class Line: public VParametrizedFunction<double ,1 ,1, 1>
{
protected:
	virtual double eval(double x, const std::vector<double>& p) const {
		return p[0]*x;
	}
};


int main() {
	Line * model = new Line();
	model->setParamValue(0, 3.);
	DataSet<double, 1, 1> * data = new DataSet<double, 1, 1>();
	data->addPoint({0.}, {0.});
	data->addPoint({1.}, {1.});
	data->addPoint({2.}, {2.});
	data->addPoint({3.}, {3.});
	data->addPoint({4.}, {4.});
	data->addPoint({5.}, {5.});

	Matrix<double, Dynamic, Dynamic> w(6, 6);
	w.setIdentity();
	w(2, 2) = 100;

	auto fitter = MakeFitter<YWChi2Fit, MnMigradMinimizer, integral_constant<int, 2>>(data, model);
	fitter->setYWeight(&w);
	cout << fitter->fit();
	delete fitter;
}



