/*
 * MinimizerFactory.hpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MINIMIZER_FACTORY_HPP
#define MINIMIZER_FACTORY_HPP

 #include "Globals.hpp"
 #include "Factory.hpp"
 #include "SingletonHolder.hpp"
 #include "Minuit2Minimizer.hpp"

 #include <iostream>

 namespace LQCDA {
 	namespace MIN {

 		template<class M>
 		class MinimizerCreator
 		{
 		public:
 			static M * create()
 			{
	 			return new M();
 			}
 		};

 		namespace {

	 		template<typename T>
 			class MinimizerFactoryImpl
 			: protected utils::Factory
 			<
 			Minimizer<T>, 
 			int,
 			std::function< Minimizer<T> * () >
 			>
 			{
  			public:
 				// Typedefs
 				typedef typename utils::Factory
 				<
 				Minimizer<T>, 
 				int,
 				std::function< Minimizer<T> * () >
 				> factory_type;
 				using typename factory_type::product_type;
 				using typename factory_type::id_type;
 				using typename factory_type::creator_type;

 			public:
 				MinimizerFactoryImpl();

 				template<typename M>
 				bool registerMinimizer(const id_type& id)
 				{
 					return factory_type::registerId(id, MinimizerCreator<M>::create);
 				}

 				std::unique_ptr<Minimizer<T>> create(
 					const MinimizerType& id)
 				{
 					return factory_type::create(id.id());
 				}

 			};

	 		template<typename T>
 			MinimizerFactoryImpl<T>::MinimizerFactoryImpl()
 			{
 				registerMinimizer<MnMigradMinimizer<T>>(MIGRAD_ID());
 			}
 		}

 		template<typename T>
 		using MinimizerFactory = utils::SingletonHolder<MinimizerFactoryImpl<T>>;

 		template<class M>
 		bool RegisterMinimizer(const typename MinimizerFactory<typename M::scalar_type>::singleton_type::id_type & id)
 		{
 			return MinimizerFactory<typename M::scalar_type>::instance().template registerMinimizer<M>(id);
 		}

 	}
 }

#endif // MINIMIZER_FACTORY_HPP