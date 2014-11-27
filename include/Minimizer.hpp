/*
 * Minimizer.hpp
 *
 *  Created on: Feb 06, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MINIMIZER_HPP
#define MINIMIZER_HPP

 #include "Globals.hpp"
 #include "Function.hpp"
 #include "ScalarConstraint.hpp"

BEGIN_NAMESPACE(LQCDA)
BEGIN_NAMESPACE(MIN)

class MinimizerOptions
{
public:
	Verbosity verbosity;

public:
	MinimizerOptions() {
		verbosity = NORMAL;
	}

	virtual ~MinimizerOptions() noexcept = default;

	virtual void print(std::ostream& os) const
	{
		os << "Minimizer options:\n"
		<< "\tverbosity = " << verbosity << std::endl;
	}
};

inline std::ostream& operator<<(std::ostream& os, const MinimizerOptions& opts)
{
	opts.print(os);
	return os;
}

template<typename T>
class Minimizer
{
public:
	struct Result
	{
		std::vector<double> minimum;
		std::vector<double> errors;
		double final_cost;
		bool is_valid;
	};

	// Typedefs
	typedef MinimizerOptions OptionsType;
	typedef T Scalar;

public:
	// Destructor
	virtual ~Minimizer() noexcept = default;

	// Options
	virtual OptionsType& options() =0;

	// Minimize
	virtual Result minimize(
		const ScalarFunction<Scalar>& F, 
		const std::vector<Scalar>& x0,
		const std::vector<Scalar>& e0,
		const std::vector<ScalarConstraint<Scalar>>& c) =0;
	Result minimize(
		const ScalarFunction<Scalar>& F, 
		const std::vector<Scalar>& x0);
};

template<typename T>
typename Minimizer<T>::Result Minimizer<T>::minimize(
		const ScalarFunction<Scalar>& F, 
		const std::vector<Scalar>& x0)
{
	return minimize(F, x0, std::vector<ScalarConstraint<Scalar>>(x0.size()));
}

template<typename T>
std::ostream& operator<< (std::ostream& out, const typename Minimizer<T>::Result& result)
{
	out << "Final cost = " << result.final_cost << std::endl;
	return out;
}

END_NAMESPACE
END_NAMESPACE

#endif // MINIMIZER_HPP