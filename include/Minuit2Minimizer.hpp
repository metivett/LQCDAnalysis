/*
 * Minuit2Minimizer.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef MINUIT2_MINIMIZER_HPP_
#define MINUIT2_MINIMIZER_HPP_

 #include "Minuit2/FCNBase.h"
 #include "Minuit2/FunctionMinimum.h"
 #include "Minuit2/MnMigrad.h"
 #include "Minuit2/MnPrint.h"
 #include "Minuit2/MnSimplex.h"
 #include "Minuit2/MnUserParameters.h"
 #include "Minimizer.hpp"


 namespace LQCDA {

 	namespace internal {

 		template<typename Fcn>
 		class Mn2FCNWrapper: public ROOT::Minuit2::FCNBase
 		{
 		private:
 			const Fcn * _F;

 		public:
 			Mn2FCNWrapper(const Fcn * f): FCNBase(), _F(f) {}

 			virtual double Up () const { return 1.0; }
 			virtual double operator()(const std::vector<double>& args) const { return (*_F)(args); }
 		};

 	}

 	template<typename Fcn, typename Args, typename L>
 	struct MnMigradMinimizer
 	{
 		static ROOT::Minuit2::FunctionMinimum minimize(const Fcn * F, const Args& initargs) {
 			ROOT::Minuit2::MnUserParameters params(initargs, std::vector<double>(initargs.size(), 0.1));
 			auto MnF = new internal::Mn2FCNWrapper<Fcn>(F);
 			ROOT::Minuit2::MnMigrad migrad(*MnF, params, L::value);
 			auto result =  migrad();
 			delete MnF;
 			return result;
 		}
 	};


 	// class Mn2Minimizer
 	// {
 	// protected:
 	// 	static MnUserParameters GetMnUserParameters(const ModelParameters& params);
 	// 	static ModelParameters GetModelParameters(const MnUserParameters& params);
 	// };

  //   template<class Fcn>
 	// class Mn2MigradMinimizer : public Mn2Minimizer
 	// {
 	// public:
 	// 	static ModelParameters Minimize(const Fcn& F, const ModelParameters& params) {
 	// 		MnUserParameters initPar = GetMnUserParameters(params);

	 //    // Pre-minimizer call
 	// 		MnMigrad Migrad1(F, initPar, 2);
 	// 		FunctionMinimum Min = Migrad1();

 	// 		LQCDDebug(3)<<"(MINUIT) Pre-minimizer call :\n"
 	// 		<< "--------------------------------------------------------"
 	// 		<< Min
 	// 		<< "--------------------------------------------------------"
 	// 		<< std::endl;

	 //    // Minimizer call
 	// 		MnUserParameters pre_min_par = Min.UserParameters();
 	// 		MnMigrad Migrad2(F, pre_min_par, 2);
 	// 		Min = Migrad2();

 	// 		if(!Min.IsValid()) {
 	// 			LQCDOut<<"Minuit Library reported that minimization result is not valid !\n";
 	// 			return GetModelParameters(Min.UserParameters());
 	// 		}
 	// 		else {
 	// 			LQCDDebug(2)<< "(MINUIT) Fit successful !\n"
 	// 			<< "Resulting minimum is : "
 	// 			<< F
 	// 			<< std::endl;
 	// 			LQCDDebug(3)<< "(MINUIT) Minimizer call :\n"
 	// 			<< "--------------------------------------------------------"
 	// 			<< Min
 	// 			<< "--------------------------------------------------------"
 	// 			<< std::endl;

 	// 			return GetModelParameters(Min.UserParameters());
 	// 		}
 	// 	}
 	// };

  //   template<class Fcn>
 	// class Mn2SimplexMinimizer : public Mn2Minimizer
 	// {
 	// public:
 	// 	static ModelParameters Minimize(const Fcn& F, const ModelParameters& params) {
 	// 		MnUserParameters initPar = GetMnUserParameters(params);

	 //    // Pre-minimizer call
 	// 		MnSimplex Migrad1(F, initPar, 2);
 	// 		FunctionMinimum Min = Migrad1();

 	// 		LQCDDebug(3)<<"(MINUIT) Pre-minimizer call :\n"
 	// 		<< "--------------------------------------------------------"
 	// 		<< Min
 	// 		<< "--------------------------------------------------------"
 	// 		<< std::endl;

	 //    // Minimizer call
 	// 		MnUserParameters pre_min_par = Min.UserParameters();
 	// 		MnSimplex Migrad2(F, pre_min_par, 2);
 	// 		Min = Migrad2();

 	// 		if(!Min.IsValid()) {
 	// 			LQCDOut<<"Minuit Library reported that minimization result is not valid !\n";
 	// 			return GetModelParameters(Min.UserParameters());
 	// 		}
 	// 		else {
 	// 			LQCDDebug(1)<< "(MINUIT) Fit successful !\n"
 	// 			<< "Resulting minimum is : "
 	// 			<< F
 	// 			<< std::endl;
 	// 			LQCDDebug(3)<< "(MINUIT) Minimizer call :\n"
 	// 			<< "--------------------------------------------------------"
 	// 			<< Min
 	// 			<< "--------------------------------------------------------"
 	// 			<< std::endl;

 	// 			return GetModelParameters(Min.UserParameters());
 	// 		}
 	// 	}
 	// };

} // namespace LQCDA

#endif	// MINUIT2_MINIMIZER_HPP_
