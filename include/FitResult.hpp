/*
 * FitResult.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESULT_HPP_
#define FIT_RESULT_HPP_

 #include "Globals.hpp"
 #include "XYData.hpp"
 #include "ParametrizedFunction.hpp"

 namespace LQCDA {
 	namespace internal {
	 	template<
	 	typename T, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
 		class FitImpl;
 	}

 	// Fit result
 	template<typename T>
 	class FitResult
 	{
 		template<
	 	typename S, 
	 	template<typename> class COST,
	 	template<typename> class MINIMIZER
	 	>
	 	friend class internal::FitImpl;

 	public:
 		FitResult()
 		: _Model{}
 		, _Cost{0}
 		, _nDOF{0}
 		, _isValid{false}
 		{}
 		// Accessors
 		const T& p(unsigned int i) const { return _Params[i]; }
 		const std::vector<T>& parameters() const { return _Params; }
 		const T& err(unsigned int i) const { return _Errors[i]; }
 		const std::vector<T>& errors() const { return _Errors; }

 		const std::vector<const ParametrizedScalarFunction<T>*>& model() const { return _Model; }
 		const ParametrizedScalarFunction<T>& model(const unsigned int k) const { return _Model[k]; }
 		
 		T cost() const { return _Cost; }
 		unsigned int nDOF() const { return _nDOF; }
 		bool isValid() const { return _isValid; }

 	private:
 		// Fitted parameters
 		std::vector<T> _Params;
 		// Errors
 		std::vector<T> _Errors;
 		// Model
 		std::vector<const ParametrizedScalarFunction<T>*> _Model;
 		// Cost function value
 		T _Cost;
 		// Number of degrees of freedom
 		unsigned int _nDOF;
 		// Validity
 		bool _isValid;
 	};


 	// std::ostream& operator<< (std::ostream& out, const FitResult& res) {
 	// 	out << "\nFitted parameters:\n"
 	// 	<< '\n';

 	// 	return out;
 	// }

} // namespace LQCDA

#endif	// FIT_RESULT_HPP_
