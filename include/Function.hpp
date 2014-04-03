/*
 * Function.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

 #include <functional>

 #include "Globals.hpp"
 #include "TypeTraits.hpp"
 #include "Exceptions.hpp"

namespace LQCDA {

/******************************************************************************
 *                               ScalarFunction                               *
 ******************************************************************************/

	template<typename T>
 	class ScalarFunction
 	{
 	private:
 		unsigned int _xDim;

 	public:
 		// Constructors/Destructor
 		explicit ScalarFunction(unsigned int xdim)
 		: _xDim(xdim)
 		{}
 		virtual ~ScalarFunction() = default;

 		// Accessors
 		unsigned int xDim() const { return _xDim; }
 		unsigned int yDim() const { return 1; }

 		// Evaluators
 		virtual T operator()(const T * x) const =0;
 		T operator()(const std::vector<T>& x) const;
 		T operator()(const Vector<T>& x) const;
 		template<typename... Ts>
 		T operator()(const Ts...x) const;

 	private:
 		void checkXdim(unsigned int xdim) const;

 	protected:
 		void setXDim(unsigned int xdim) { _xDim = xdim; }

 	};

 	template<typename T>
 	T ScalarFunction<T>::operator()(const std::vector<T>& x) const
 	{
 		checkXdim(x.size());
 		return (*this)(x.data());
 	}
 	template<typename T>
 	T ScalarFunction<T>::operator()(const Vector<T>& x) const
 	{
 		checkXdim(x.size());
 		return (*this)(x.data());
 	}
 	template<typename T>
 	template<typename... Ts>
 	T ScalarFunction<T>::operator()(const Ts... xs) const
 	{
 		// static_assert(are_trivially_assignable<T, Ts...>::value, 
 		// 	"ScalarFunction provided arguments are not compatible with type T");
 		static_assert(are_floating_points<Ts...>::value, 
 			"ScalarFunction provided arguments are not compatible with type T");

 		const T x[] = {xs...};
 		checkXdim(sizeof...(xs));
 		return (*this)(x);
 	}

 	template<typename T>
 	void ScalarFunction<T>::checkXdim(unsigned int xdim) const
 	{
 		if(_xDim && xdim != xDim()) {
 			ERROR(SIZE, "wrong number of arguments provided (expected "
 				+ utils::strFrom(xDim()) + ", got " + utils::strFrom(xdim) + ")");
 		}
 	}


/******************************************************************************
 *                               VectorFunction                               *
 ******************************************************************************/

 	// template<typename T>
 	// class VectorFunction
 	// {
 	// private:
 	// 	unsigned int _xDim, _yDim;

 	// public:
 	// 	// Constructors/Destructor
 	// 	explicit VectorFunction(unsigned int xdim, unsigned int ydim)
 	// 	: _xDim(xdim)
 	// 	, _yDim(ydim)
 	// 	{}
 	// 	virtual ~VectorFunction() = default;

 	// 	// Accessors
 	// 	unsigned int xDim() const { return _xDim; }
 	// 	unsigned int yDim() const { return _yDim; }

 	// 	// Evaluators
 	// 	virtual T operator()(const T * x) const =0;
 	// 	T operator()(const std::vector<T>& x) const;
 	// 	T operator()(const Vector<T>& x) const;
 	// 	template<typename... Ts>
 	// 	T operator()(const Ts...x) const;

 	// private:
 	// 	void checkXdim(unsigned int xdim) const;

 	// };

 }

#endif // FUNCTION_HPP