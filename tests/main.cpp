#include "LQCDA.hpp"
#include "Function.hpp"
#include "GSLRootFinder.hpp"

#include <iostream>
#include <vector>
#include <type_traits>

using namespace LQCDA;
// using namespace PLOT;
using namespace std;
//using namespace ROOT::Minuit2;

class Line
    : public LQCDA::ParametrizedSFunction<double>
{
public:
    Line()
        : ParametrizedSFunction<double>(1, 2, std::function<double(const double *, const double *)>(eval))
    {}

    using LQCDA::ParametrizedSFunction<double>::operator();

private:
    static double eval(const double *x, const double *p)
    {
        // a*x+b
        return p[0] * (*x) + p[1];
    }
};

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
    double x2 = (a - b) * (a - b) + b * b;
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

    // auto myF = SFunction<double(double, double)>(f);
    // cout << myF(1, 2) << endl;
    // auto myBoundF = LQCDA::bind(myF, 1, std::placeholders::_1);
    // cout << myBoundF(2) << endl;

    // MIN::MIGRAD<double> myMinimizer;
    // myMinimizer.options().error_definition = 1.e-3;
    // auto myFMin = myMinimizer.minimize(myF, {0., 0.});

    // XYData<double> xyd(5, 1, 1);
    // xyd.y({}, 0) << 2., 3., 4., 5., 6.;
    // xyd.x({}, 0) << 0., 1., 2., 3., 4.;
    // FOR_MAT_DIAG(xyd.yyCov(0,0), i)
    // {
    //     xyd.yyCov(0, 0)(i, i) = 0.1;
    // }
    // cout << xyd.yyCov({}, {}) << endl;
    // Line myLine;
    // Chi2Fit<double, MIN::MIGRAD> myFit(xyd);
    // myFit.fitAllPoints(true);
    // myFit.options.verbosity = DEBUG;
    // ScalarConstraint<double> myAConstraint, myBConstraint;
    // myAConstraint.fixValue(1.);
    // auto myFitResult = myFit.fit(myLine, {0., 0.}, {myAConstraint, myBConstraint});
    // for(double p: myFitResult.parameters())
    //     cout << p << endl;

    Matrix<double> mat(4, 3);
    mat << 1., 1., 1., 2., 2., 1., 3., 2., 1., 4., 5., 1.;

    cout << mat << endl << endl;
    cout << PseudoInverse(mat) << endl;

    Sample<double> mySample(4);
    FOR_SAMPLE(mySample, s)
    {
        mySample[s] = s;
    }
    cout << "mean\n" << mySample.mean() << endl;
    cout << "median\n" << mySample.median() << endl;
    cout << "variance\n" << mySample.variance() << endl;
    cout << "MAD\n" << mySample.medianDeviation() << endl;
    cout << "stddev\n" << mySample.standardDeviation() << endl;
    cout << endl;

    Sample<double> mySample2(4);
    FOR_SAMPLE(mySample2, s)
    {
        mySample2[s] = s+1;
    }
    cout << "mean\n" << mySample2.mean() << endl;
    cout << "median\n" << mySample2.median() << endl;
    cout << "variance\n" << mySample2.variance() << endl;
    cout << "MAD\n" << mySample2.medianDeviation() << endl;
    cout << "stddev\n" << mySample2.standardDeviation() << endl;
    cout << endl;

    Sample<Matrix<double>> myMatSample(4, 2, 1);
    FOR_SAMPLE(myMatSample, s)
    {
        myMatSample[s] << s, s+1;
    }
    cout << "mean\n" << myMatSample.mean() << endl;
    cout << "median\n" << myMatSample.median() << endl;
    cout << "variance\n" << myMatSample.variance() << endl;
    cout << "MAD\n" << myMatSample.medianDeviation() << endl;
    cout << "stddev\n" << myMatSample.standardDeviation() << endl;
    cout << endl;
    cout << "block(0,0)" << endl;
    cout << "mean\n" << myMatSample.block(0, 0, 1, 1).mean() << endl;
    cout << "median\n" << myMatSample.block(0, 0, 1, 1).median() << endl;
    cout << "variance\n" << myMatSample.block(0, 0, 1, 1).variance() << endl;
    cout << "MAD\n" << myMatSample.block(0, 0, 1, 1).medianDeviation() << endl;
    cout << "stddev\n" << myMatSample.block(0, 0, 1, 1).standardDeviation() << endl;
    cout << endl;
    cout << "elem(0,0)" << endl;
    cout << "mean\n" << myMatSample(0, 0).mean() << endl;
    cout << "median\n" << myMatSample(0, 0).median() << endl;
    cout << "variance\n" << myMatSample(0, 0).variance() << endl;
    cout << "MAD\n" << myMatSample(0, 0).medianDeviation() << endl;
    cout << "stddev\n" << myMatSample(0, 0).standardDeviation() << endl;
}



