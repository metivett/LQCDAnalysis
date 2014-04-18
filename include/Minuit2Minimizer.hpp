/*
 * Minuit2Minimizer.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef MINUIT2_MINIMIZER_HPP_
#define MINUIT2_MINIMIZER_HPP_

 #include "Minimizer.hpp"

 #include "Minuit2/FCNBase.h"
 #include "Minuit2/FunctionMinimum.h"
 #include "Minuit2/MnMigrad.h"
 #include "Minuit2/MnPrint.h"
 #include "Minuit2/MnSimplex.h"
 #include "Minuit2/MnUserParameters.h"


 namespace LQCDA {
 	namespace MIN {

 		template<typename T>
 		class MnMigradMinimizer;

 		template<typename T>
 		using MIGRAD = MnMigradMinimizer<T>;

 		template<>
 		class Options<MIGRAD>
 		: public Options<DEFAULT>
 		{
 		public:
 			unsigned int level;
 			bool pre_minimize;
 			unsigned int pre_min_level;

 		public:
 			Options()
 			: Options<DEFAULT>()
 			{ 
 				init(); 
 			}
 			Options(const Options<DEFAULT>& opts)
 			: Options<DEFAULT>(opts)
 			{
 				// try 
 				// {
 				// 	init(dynamic_cast<const Options&>(opts));
 				// }
 				// catch(std::bad_cast) {
 				// 	WARNING("uncomplete or incompatible options provided; init with default");
 				// 	init();
 				// }
 				init();
 			}

 			virtual ~Options() noexcept = default;

 		private:
 			void init()
 			{
 				level = 2;
 				pre_minimize = true;
 				pre_min_level = 1;
 			}
 		};

 		template<typename T>
 		std::ostream& operator<<(std::ostream& os, const LQCDA::MIN::Options<MIGRAD>& opts)
 		{
 			os << "MIGRAD options:\n"
 			<< "\tlevel = " << opts.level << std::endl
 			<< "\tpre_minimize = " << opts.pre_minimize << std::endl
 			<< "\tpre_minimize level = " << opts.pre_min_level << std::endl;
 		}

 		template<typename T>
 		class MnMigradMinimizer
 		: public Minimizer<T>
 		{
 		public:
 			// Typedefs
 			typedef Options<MIGRAD> options_type;
 			typedef MIGRAD_ID ID;

 		private:
 			class Mn2FCNWrapper
 			: public ROOT::Minuit2::FCNBase
 			{
 			private:
 				const ScalarFunction<T>& _F;

 			public:
 				Mn2FCNWrapper(const ScalarFunction<T>& f): FCNBase(), _F(f) {}

 				virtual double Up () const { return 1.0; }
 				virtual double operator()(const std::vector<double>& args) const { 
 					return _F(args);
 				}
 			};

 		private:
 			options_type _Opts;

 		public:
 			// Constructor
 			MnMigradMinimizer(const options_type& opts = options_type())
 			: _Opts(opts)
 			{}
 			// Destructor
 			virtual ~MnMigradMinimizer() noexcept = default;

 			// Options
 			virtual options_type& options() override { return _Opts; }

 			// Minimize
 			virtual typename Minimizer<T>::Result minimize(
 				const ScalarFunction<T>& F,
 				const std::vector<T>& x0) override;

 		};

 		template<typename T>
 		typename Minimizer<T>::Result MnMigradMinimizer<T>::minimize(
 			const ScalarFunction<T>& F, 
 			const std::vector<T>& x0)
 		{
 			utils::vostream vout(std::cout, _Opts.verbosity);
 			vout(NORMAL) << "Minimizing with Minuit2 MIGRAD minimizer\n";
 			// vout(NORMAL) << options << std::endl;
 			LQCDA::MIN::operator<< <T>(vout(NORMAL), _Opts) << std::endl;

 			vout(NORMAL) << "Initial parameters:\n";
 			ROOT::Minuit2::MnUserParameters params(x0, std::vector<T>(x0.size(), 0.1));
 			vout(NORMAL) << params << std::endl;

 			auto MnF = new Mn2FCNWrapper(F);

 			if(_Opts.pre_minimize)
 			{
 				vout(DEBUG) <<"(MINUIT) Pre-minimizer call :\n"
 				<< "--------------------------------------------------------";
 				ROOT::Minuit2::MnMigrad migrad1(*MnF, params, _Opts.pre_min_level);
 				auto preMin = migrad1();
 				vout(DEBUG) << preMin
 				<< "--------------------------------------------------------"
 				<< std::endl;

 			}
 			vout(DEBUG) <<"(MINUIT) Minimizer call :\n"
 			<< "--------------------------------------------------------";
 			ROOT::Minuit2::MnMigrad migrad2(*MnF, params, _Opts.level);
 			auto Min =  migrad2();

 			delete MnF;

 			typename Minimizer<T>::Result result;
 			result.final_cost = Min.Fval();
 			result.is_valid = Min.IsValid();
 			result.minimum = Min.UserParameters().Params();

 			if(!Min.IsValid()) {
 				vout(NORMAL) << "Minuit Library reported that minimization result is not valid !\n";
 				vout(DEBUG)
 				<< "--------------------------------------------------------"
 				<< Min
 				<< "--------------------------------------------------------"
 				<< std::endl;

 				return result;
 			}
 			else {
 				vout(NORMAL) << "(MINUIT) Fit successful !\n"
 				<< "Resulting minimum is : " << std::endl
 				<< "--------------------------------------------------------"
 				<< Min
 				<< "--------------------------------------------------------"
 				<< std::endl;

 				return result;
 			}
 		}


 	// template<typename T>
 	// void RegisterMnMigradMinimizer()
 	// {
 	// 	static MinimizerFactoryRegistrar<MnMigradMinimizer, T> MFR;
 	// }

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

 	}
} // namespace LQCDA


#endif	// MINUIT2_MINIMIZER_HPP_
