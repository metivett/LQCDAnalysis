#include "Fit.hpp"
#include "Minimize.hpp"
#include "FitResult.hpp"
#include "Minuit2Minimizer.hpp"
#include "CostFunction.hpp"
#include "Sample.hpp"
#include "MatrixSample.hpp"

#include "DataFile.hpp"

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
	// Line * model = new Line();
	// model->setParameter(0, 3.);
	// XYData<double> * data = new XYData<double>(5, 1, 1);
	// data->x({}, 0) << 0, 1, 2, 3, 4;
	// data->y({}, 0) << 0, 1, 2, 3, 4;

	// std::vector<double> xinit = {0.};
	
	// auto fit = Chi2Fit<double, MIN::MIGRAD>(*data).fit(*model, xinit);
	// std::cout << fit.cost() << std::endl;
	// std::cout << fit.parameters()[0] << std::endl;

	// delete model;
	// delete data;

	// F f;
	// std::vector<double> x0 {1., 1.};
	// auto mini = MIN::MakeMinimizer<double, MIN::MIGRAD>();
	// mini->options().level = 3;
	// auto result = mini->minimize(f, x0);
	// std::cout << result.is_valid << std::endl;
	// std::cout << result.final_cost << std::endl;

	// Sample<Matrix<double>> s1(10), s2(10);
	// for(int i=0; i<5; ++i)
	// {
	// 	s1[2*i] = Matrix<double>::Identity(2, 2);
	// 	s1[2*i+1] = Matrix<double>::Zero(2, 2);
	// 	s2[i] = Matrix<double>::Identity(2, 2);
	// }

	// Sample<double> s1(10), s2(10);
	// for(int i=0; i<10; ++i)
	// {
	// 	s1[i] = static_cast<double>(i);
	// 	s2[i] = static_cast<double>(2*i);
	// }

	// Matrix<double> array [10];
	// for(int i=0; i<10; ++i)
	// 	array[i] = Matrix<double>::Identity(3,3);

	// Sample<Matrix<double>&> s1(array, 10, 1);
	// // s1[0]=array[0].block(0,0,2,2);
	// Sample<Matrix<double>> s2(2);
	// s2[0].setIdentity(3, 3);
	// s2[1].setZero(3, 3);

	// std::cout << s2.size() << std::endl;
	// std::cout << s2.block(0, 1, 2, 2)[0] << std::endl;
	
	// std::cout << s1[0] << std::endl;
	// s1[0](1,1) = 2.;
	// std::cout << array[0] << std::endl;

	// std::cout << s1.mean() << std::endl;
	// std::cout << s1.variance() << std::endl;
	// std::cout << s1.varianceMatrix() << std::endl;
	// std::cout << s1.covariance(s2) << std::endl;

	AsciiDataFile file(std::string("test.dat"), 'r');
	Matrix<double> datamat(file.getData(std::string("test")));
	std::cout << datamat << std::endl;
}



