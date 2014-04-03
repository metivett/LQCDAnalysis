/*
 * Policies.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Thibaut Metivet
 */

#ifndef POLICIES_HPP
#define POLICIES_HPP

 #include "Exceptions.hpp"

 namespace utils {
 	namespace policies {

 		namespace creation {

 			template<class T>
 			struct OpNewCreator
 			{
 				static T * create()
 				{
 					return new T;
 				}
 				static void destroy(T * t)
 				{
 					delete t;
 				}
 			};

 		}

 		namespace lifetime {

 			template<class T>
 			struct DefaultLifetime
 			{	
 				static void onDeadReference()
 				{
 					ERROR(RUNTIME, "Dead reference detected");
 				}
 				typedef void (*destructor)();
 				static void scheduleDestruction(destructor d)
 				{
 					atexit(d);
 				}

 			};

 		}

 		namespace thread {

 			template<class T>
 			class SingleThreaded
 			{
 			public:
 				// Typedefs
 				typedef T volatile_type;

 				// Lock
 				class Lock {};

 			};

 		}
 	}
 }

#endif // POLICIES_HPP