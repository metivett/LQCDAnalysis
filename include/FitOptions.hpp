/*
 * FitOptions.hpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FIT_OPTIONS_HPP
#define FIT_OPTIONS_HPP

 #include <type_traits>

 namespace LQCDA {

 	template<class XT>
 	struct FitRange
 	{
 		XT Xmin, Xmax;

 		FitRange(const XT& xmin, const XT& xmax): Xmin(xmin), Xmax(xmax) {}
 	};

 	template<class XT, class YT>
 	class FitOptions
 	{
 	public:
 		typedef YT y_type;
 		typedef typename std::remove_reference<XT>::type x_type;

 		enum MinimizerType {
 			Mn2Migrad, Mn2Simplex
 		};
 		enum FitMethod {
 			LeastSquares
 		};

 	private:
 		MinimizerType _MinimizerType;
 		FitMethod _FitMethod;
 		FitRange<x_type> * _FitRange;

 	public:
 		FitOptions() {
 			_MinimizerType = MinimizerType::Mn2Migrad;
 			_FitMethod = FitMethod::LeastSquares;
 			_FitRange = 0;
 		}

 		MinimizerType GetMinimizerType() const { return _MinimizerType; }
 		FitMethod GetFitMethod() const { return _FitMethod; }

 		bool HasFitRange() const { return _FitRange != 0; }
 		const FitRange<x_type>& GetFitRange() const { assert(HasFitRange()); return _FitRange; }

 		void SetMinimizerType(MinimizerType t) { _MinimizerType = t; }
 		void SetFitMethod(FitMethod m) { _FitMethod = m; }
 		void SetFitRange(const FitRange<x_type>& r) {
 			if(_FitRange){
 				_FitRange->Xmin = r.Xmin; _FitRange->Xmax = r.Xmax;
 			}
 			else
 				_FitRange = new FitRange<x_type>(r);
 		}

 	};

 }

#endif // FIT_OPTIONS_HPP