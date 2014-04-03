/*
 * FactoryError.hpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FACTORY_ERROR_HPP
#define FACTORY_ERROR_HPP

 #include <memory>

 #include "Exceptions.hpp"

 namespace utils {

 	template
 	<
 	typename Identifier,
 	class Product
 	>
 	class DefaultFactoryError
 	{
 	public:
 		class Exception
 		: public std::logic_error
 		{
 		private:
 			Identifier _id;
 			
 		public:
 			Exception(const Identifier& id)
 			: _id(id)
 			, std::logic_error("factory error: unknown type " + MAKE_STRING(Identifier))
 			{}

 			const Identifier id() const { return _id; }
 		};

 	public:
 		static std::unique_ptr<Product> onUnknownType(const Identifier& id)
 		{
 			throw Exception(id);
 		}
 	};

 }

#endif // FACTORY_ERROR_HPP