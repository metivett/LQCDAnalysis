// #include "LQCDA.hpp"
#include "Function.hpp"
#include "GSLRootFinder.hpp"

#include <iostream>
#include <vector>
#include <type_traits>

using namespace LQCDA;
// using namespace PLOT;
using namespace std;
//using namespace ROOT::Minuit2;

// class Line
//     : public LQCDA::ParametrizedScalarFunction<double>
// {
// public:
//     Line()
//         : ParametrizedScalarFunction<double>(1, 1)
//     {}

//     using LQCDA::ParametrizedScalarFunction<double>::operator();
//     virtual double operator()(const double *x, const double *p) const
//     {
//         return 2.*(*p) * (*x);
//     }
// };

// class F
//     : public LQCDA::ScalarFunction<double>
// {
// public:
//     F()
//         : ScalarFunction<double>(2)
//     {}

//     using ScalarFunction<double>::operator();

// public:
//     virtual double operator()(const double *x) const override
//     {
//         double x2 = x[0] * x[0] + x[1] * x[1];
//         return x2 - 2 * x[0] + exp(x[1]) + 10.;
//     }
// };

// class FF
// {
// public:
//     FF()
//     {}

// public:
//     virtual double operator()(const double *x) const
//     {
//         double x2 = x[0] * x[0] + x[1] * x[1];
//         return x2 - 2 * x[0] + exp(x[1]) + 10.;
//     }
// };

double f(double a, double b)
{
    double x2 = a * a + b * b;
    return x2 - 2 * a + exp(b) + 10.;
}

double g(double x)
{
    return x - 1.;
}


int main()
{
    // Array<double, Dynamic, 3> xydy(4, 3);
    // xydy << 1., 1., 1., 2., 2., 1., 3., 2., 1., 4., 5., 1.;
    // GracePlot plot;
    // plot.G(0) << PlotXYDY(xydy);
    // plot.G(0) << Title("Test");
    // plot.G(0).S(0) << Color(ColorType::red);
    // plot.display();
    // std::string tmp;
    // std::cin >> tmp;

    // Sample<double> s1(10);
    // s1 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;
    // Sample<double> s2(10);
    // s2 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;
    // cout << s1.covarianceMatrix(s2) << endl;
    // cout << s1 - 1 << endl;
    // cout << endl;

    // Sample<Matrix<double>> s3(2);
    // Matrix<double> m1 = Matrix<double>::Constant(1, 1, 1);
    // Matrix<double> m2 = Matrix<double>::Constant(1, 1, 2);
    // s3 << m1, m2;
    // s3 += 2.;
    // cout << (s3)[0] << endl;

    // Matrix<int> m(3, 3);
    // m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    // cout << m << endl << endl;
    // cout << m.view(Eigen::Range<2>(0, 2), Eigen::Range<2>(0, 2)) << endl << endl;
    // cout << m.view(0, Eigen::Range<2>(0, 2)) << endl << endl;
    // cout << m.view(Eigen::Range<2>(0, 2), 1) << endl << endl;
    
    // F myF;
    // std::vector<double> p = {1., 2.};
    // cout << myF(p) << endl;
    // auto myBoundF = myF.bind(std::placeholders::_2, std::placeholders::_1);
    // double d = myBoundF(2., 1.);
    // cout << d << endl;
    
    // Line myLine;
    // const double x = 1., p = 2.;
    // cout << myLine(&x, &p) << endl;
    // auto myBoundLine = myLine.bind(std::placeholders::_1, 2);
    // cout << myBoundLine(1) << endl;
    
    auto myF = Function<double(double, double)>(f);
    cout << myF(1, 2) << endl;
    auto myBoundF = LQCDA::bind(myF, 1, std::placeholders::_1);
    cout << myBoundF(2) << endl;

    LQCDA::Roots::BrentRootFinder<double> brf;
    auto root = brf.solve(Function<double(double)>(g), 0., 2.);
    cout << root.value() << endl;
}



