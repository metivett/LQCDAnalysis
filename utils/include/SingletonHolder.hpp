/*
 * SingletonHolder.hpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Thibaut Metivet
 *  from A.Alexandrescu's Modern C++ 
 */

#ifndef SINGLETON_HOLDER_HPP
#define SINGLETON_HOLDER_HPP

 #include "Utilities.hpp"
 #include "Policies.hpp"

 namespace utils {

 	template
 	<
 	class T,
 	template<class> class CreationPolicy = policies::creation::OpNewCreator,
 	template<class> class LifetimePolicy = policies::lifetime::DefaultLifetime,
 	template<class> class ThreadModel = policies::thread::SingleThreaded
 	>
 	class SingletonHolder
 	{
 	public:
 		// Typedefs
 		typedef T singleton_type;

 	private:
 		// Typedefs
 		typedef typename ThreadModel<T*>::volatile_type instance_ptr_type;

 		// Data
 		static instance_ptr_type _Instance;
 		static bool _Destroyed;

 	public:
 		static T& instance();

 	private:
 		// Protection
 		SingletonHolder();
 		SingletonHolder(const SingletonHolder&);
 		SingletonHolder& operator=(const SingletonHolder&);
 		~SingletonHolder();

 		// Helpers
 		static void destroy();
 	};

 	template
 	<
 	class T,
 	template<class> class CreationPolicy,
 	template<class> class LifetimePolicy,
 	template<class> class ThreadModel
 	>
 	typename SingletonHolder<T, CreationPolicy, LifetimePolicy, ThreadModel>::instance_ptr_type 
 	SingletonHolder<T, CreationPolicy, LifetimePolicy, ThreadModel>::_Instance;

 	template
 	<
 	class T,
 	template<class> class CreationPolicy,
 	template<class> class LifetimePolicy,
 	template<class> class ThreadModel
 	>
 	bool SingletonHolder<T, CreationPolicy, LifetimePolicy, ThreadModel>::_Destroyed;


 	template
 	<
 	class T,
 	template<class> class CreationPolicy,
 	template<class> class LifetimePolicy,
 	template<class> class ThreadModel
 	>
 	T& SingletonHolder<T, CreationPolicy, LifetimePolicy, ThreadModel>::instance()
 	{
 		if(!_Instance)
 		{
 			typename ThreadModel<T>::Lock guard;
 			if(!_Instance)
 			{
 				if(_Destroyed)
 				{
 					LifetimePolicy<T>::onDeadReference();
 					_Destroyed = false;
 				}
 				_Instance = CreationPolicy<T>::create();
 				LifetimePolicy<T>::scheduleDestruction(&destroy);
 			}
 		}
 		return *_Instance;
 	}

 	template
 	<
 	class T,
 	template<class> class CreationPolicy,
 	template<class> class LifetimePolicy,
 	template<class> class ThreadModel
 	>
 	void SingletonHolder<T, CreationPolicy, LifetimePolicy, ThreadModel>::destroy()
 	{
 		ASSERT(!_Destroyed);
 		CreationPolicy<T>::destroy(_Instance);
 		_Instance = 0;
 		_Destroyed = true;
 	}

 }

#endif // SINGLETON_HOLDER_HPP
