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

 #include <iostream>

 namespace LQCDA {

 	template<typename T>
 	class Minimizer;
 	template<typename T>
 	class MnMigradMinimizer;

 	template<class M>
 	class MinimizerCreator
 	{
 	public:
 		static M * create(
 			const typename Minimizer<typename M::scalar_type>::Options& opts)
 		{
 			return new M(opts);
 		}
 	};

 	namespace {

	 	template<typename T>
	 	class MinimizerFactoryImpl
	 	: public utils::Factory
	 	<
	 	Minimizer<T>, 
	 	MinimizerID,
	 	std::function< Minimizer<T> * (const typename Minimizer<T>::Options&) >
	 	>
	 	{
	 	public:
 			// Typedefs
 			typedef typename utils::Factory
				 	<
				 	Minimizer<T>, 
				 	MinimizerID,
				 	std::function< Minimizer<T> * (const typename Minimizer<T>::Options&) >
				 	> factory_type;
 			using typename factory_type::product_type;
 			using typename factory_type::id_type;
 			using typename factory_type::creator_type;

	 	public:
	 		MinimizerFactoryImpl();

	 		std::unique_ptr<Minimizer<T>> create(
	 			const id_type& id, 
	 			const typename Minimizer<T>::Options& opts)
	 		{
	 			return factory_type::template create<const typename Minimizer<T>::Options&>(id, opts);
	 		}

	 	};

	 	template<typename T>
	 	MinimizerFactoryImpl<T>::MinimizerFactoryImpl()
	 	{
	 		this->registerId(MinimizerType::MIGRAD, MinimizerCreator<MnMigradMinimizer<T>>::create);
	 	}
 	}

 	template<typename T>
 	using MinimizerFactory = utils::SingletonHolder<MinimizerFactoryImpl<T>>;

 	template<class M>
 	bool RegisterMinimizer(const typename MinimizerFactory<typename M::scalar_type>::singleton_type::id_type & id)
 	{
 		return MinimizerFactory<typename M::scalar_type>::instance().registerId(id, MinimizerCreator<M>::create);
 	}

 }

#endif // MINIMIZER_FACTORY_HPP