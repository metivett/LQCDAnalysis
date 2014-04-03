#include "Fit.hpp"
#include "FitResult.hpp"
#include "Minuit2Minimizer.hpp"
#include "XYFitData.hpp"
#include "CostFunction.hpp"
#include <iostream>
#include <vector>
#include <type_traits>

using namespace LQCDA;
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

template<typename T>
 	std::ostream& operator<<(std::ostream& os, const typename MnMigradMinimizer<T>::Options& opts)
 	{
 		os << "MIGRAD options:\n"
 		<< "\tlevel = " << opts.level << std::endl
 		<< "\tpre_minimize = " << opts.pre_minimize << std::endl
 		<< "\ttpre_minimize level = " << opts.pre_min_level << std::endl;
 	}


int main() {
	Line * model = new Line();
	model->setParameter(0, 3.);
	XYFitData<double> * data = new XYFitData<double>(5, 1, 1);
	// data->x(0, {}) << 0.;
	// data->x(1, {}) << 1.;
	// data->x(2, {}) << 2.;
	// data->x(3, {}) << 3.;
	// data->x(4, {}) << 4.;
	// data->y(0, {}) << 0.;
	// data->y(1, {}) << 1.;
	// data->y(2, {}) << 2.;
	// data->y(3, {}) << 3.;
	// data->y(4, {}) << 4.;
	data->x({}, 0) << 0, 1, 2, 3, 4;
	data->y({}, 0) << 0, 1, 2, 3, 4;
	// data->addPoint({0.}, {0.});
	// data->addPoint({1.}, {1.});
	// data->addPoint({2.}, {2.});
	// data->addPoint({3.}, {3.});
	// data->addPoint({4.}, {4.});
	// data->addPoint({5.}, {5.});
	std::vector<double> x {0., 1., 2., 3., 4., 5.};
	std::vector<double> y {0., 1., 2., 3., 4., 5.};

	std::vector<double> xinit = {0.};
	// Fitter<double> fitter(*data);
	// fitter.fit(*model, xinit);
	auto fit = Fit(*data, *model, xinit);
	std::cout << fit.cost() << std::endl;
	std::cout << fit.parameters()[0] << std::endl;

	// cout << fit<Chi2Fit, MnMigradMinimizer>(model, x, y);

	// Matrix<double, Dynamic, Dynamic> w(6, 6);
	// w.setIdentity();
	// w(2, 2) = 100;

	// auto fitter = MakeFitter<YWChi2Fit, MnMigradMinimizer, integral_constant<int, 2>>(data, model);
	// fitter->setYWeight(&w);
	// cout << fitter->fit();
	// delete fitter;
	delete model;
	delete data;

	F f;
	std::vector<double> x0 {1., 1.};
	//const MinimizerCreator<LQCDA::MnMigradMinimizer, double, const int> creator(MinimizerType::MIGRAD);
	//MinimizerFactory<double>::instance().registerId(MinimizerType::MIGRAD, createMigrad);
	MnMigradMinimizer<double>::Options opts;
	opts.level = 3;
	auto result = Minimize(f, x0, MinimizerType::MIGRAD, opts);
	std::cout << result.is_valid << std::endl;
	std::cout << result.final_cost << std::endl;
	// std::cout << result.minimum[0] << ", " << result.minimum[1] << std::endl;
	//std::cout << - std::numeric_limits<double>::min() << std::endl;

	// XYFitData<double> fitdata;
	// fitdata.resize(2, 2, 2);
	// Chi2CostFunction<double> chi2(fitdata);
}



