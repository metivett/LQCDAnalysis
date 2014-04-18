/*
 * Factory.hpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FACTORY_HPP
#define FACTORY_HPP

 #include <map>
 #include <functional>
 #include <memory>

 #include "FactoryError.hpp"

 namespace utils {

 	template
 	<
 	class Product,
 	typename Identifier,
 	typename Creator = std::function<Product * ()>,
 	template<typename, class> class FactoryErrorPolicy = DefaultFactoryError
 	>
 	class Factory
 	: public FactoryErrorPolicy<Identifier, Product>
 	{
 	public:
 		// Typedefs
 		typedef Product product_type;
 		typedef Identifier id_type;
 		typedef Creator creator_type;

 	private:
 		// Typedefs
 		typedef std::map<Identifier, Creator> factory_map;

 		// Data
 		factory_map _FactoryMap;

 	public:
 		// Constructor
 		Factory() = default;
 		// Destructor
 		~Factory() = default;

 		// Registration
 		bool registerId(const Identifier& id, Creator c)
 		{
 			return _FactoryMap.insert({id, c}).second;
 		}
 		bool unregisterId(const Identifier& id)
 		{
 			return _FactoryMap.erase(id) == 1;
 		}

 		// Creation
 		template<typename... ARGS>
 		std::unique_ptr<Product> create(const Identifier& id, ARGS... args)
 		{
 			auto i = _FactoryMap.find(id);
 			if(i != _FactoryMap.end())
 				return std::unique_ptr<Product>((i->second)(args...));
 			
 			return this->onUnknownType(id);
 		}

 	protected:
 		Creator& getCreator(const Identifier& id)
 		{
 			auto i = _FactoryMap.find(id);
 			if(i != _FactoryMap.end())
 				return i->second;
 			
 			return this->onUnknownType(id);
 		}

 	};

 }

#endif // FACTORY_HPP