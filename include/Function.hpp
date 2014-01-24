/*
 * Function.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

 #include "Globals.hpp"
 #include "Vector.hpp"
 #include "Array.hpp"
 #include "TypeTraits.hpp"

 namespace LQCDA {

	// template<typename XT, typename YT>
 // 	class Function
 // 	{
 // 	public:
 // 		typedef typename std::remove_reference<XT>::type x_type;
 // 		typedef typename std::remove_reference<YT>::type y_type;

 // 	protected:
 // 		typedef typename if_<std::is_fundamental<x_type>::value, x_type, const x_type&>::result x_arg_type;
 // 		typedef typename if_<std::is_fundamental<y_type>::value, y_type, const y_type&>::result y_arg_type;

 // 	public:
 // 		virtual y_type operator() (x_arg_type) const =0;

 // 	};

 // 	template<typename T, int XDIM, int YDIM>
 // 	using VFunction = Function<Vector<T, XDIM>, Vector<T, YDIM>>;

/******************************************************************************
 *                            Base Function class                             *
 ******************************************************************************/

 	/* Generic template declaration */
 	template<typename Scalar, int XDIM, int YDIM>
 	class Function;

 	/* Compile-time X and Y dimensions */
 	template<typename Scalar, int XDIM, int YDIM>
 	class Function
 	{
 	public:
 		// Typedefs
 		typedef array<Scalar, XDIM> x_type;
 		typedef array<Scalar, YDIM> y_type;

 	protected:
 		// Typedefs
 		typedef typename if_<std::is_fundamental<x_type>::value, x_type, const x_type&>::result arg_type;

 	public:
 		// Constructors/Destructor
 		explicit Function() {}
 		virtual ~Function() {}

 		// Accessors
 		constexpr unsigned int XDim() const { return XDIM; }
 		constexpr unsigned int YDim() const { return YDIM; }

 		// Evaluators
 		y_type operator()(arg_type x) { return evaluate(x); }

 	private:
 		virtual y_type evaluate(arg_type x) =0;

 	};

 	/* Compile-time X dimension and dynamic Y dimension */
 	template<typename Scalar, int XDIM>
 	class Function<Scalar, XDIM, Dynamic>
 	{
 	public:
 		// Typedefs
 		typedef array<Scalar, XDIM> x_type;
 		typedef vector<Scalar> y_type;

 	protected:
 		// Typedefs
 		typedef typename if_<std::is_fundamental<x_type>::value, x_type, const x_type&>::result arg_type;

 	public:
 		// Constructors/Destructor
 		explicit Function() {}
 		
 		virtual ~Function() {}

 		// Accessors
 		constexpr unsigned int XDim() const { return XDIM; }
 		virtual unsigned int YDim() const =0;

 		// Evaluators
 		y_type operator()(arg_type x) { return evaluate(x); }

 	private:
 		virtual y_type evaluate(arg_type x) =0;

 	};

 	/* Compile-time Y dimension and dynamic X dimension */
 	template<typename Scalar, int YDIM>
 	class Function<Scalar, Dynamic, YDIM>
 	{
 	public:
 		// Typedefs
 		typedef vector<Scalar> x_type;
 		typedef array<Scalar, YDIM> y_type;

 	protected:
 		// Typedefs
 		typedef typename if_<std::is_fundamental<x_type>::value, x_type, const x_type&>::result arg_type;

 	public:
 		// Constructors/Destructor
 		explicit Function() {}
 		virtual ~Function() {}

 		// Accessors
 		virtual unsigned int XDim() const =0;
 		constexpr unsigned int YDim() const { return YDIM; }

 		// Evaluators
 		y_type operator()(arg_type x) { return evaluate(x); }

 	private:
 		virtual y_type evaluate(arg_type x) =0;

 	};

 	/* Dynamic X and Y dimensions */
 	template<typename Scalar>
 	class Function<Scalar, Dynamic, Dynamic>
 	{
 	public:
 		// Typedefs
 		typedef vector<Scalar> x_type;
 		typedef vector<Scalar> y_type;

 	protected:
 		// Typedefs
 		typedef typename if_<std::is_fundamental<x_type>::value, x_type, const x_type&>::result arg_type;

 	public:
 		// Constructors/Destructor
 		explicit Function() {}
 		virtual ~Function() {}

 		// Accessors
 		virtual unsigned int XDim() const =0;
 		virtual unsigned int YDim() const =0;

 		// Evaluators
 		y_type operator()(arg_type x) { return evaluate(x); }

 	private:
 		virtual y_type evaluate(arg_type x) =0;

 	};

 	#define INHERIT_FUNCTION_TYPEDEFS \
 	public: \
 		using typename Function<Scalar, XDIM, YDIM>::x_type; \
 		using typename Function<Scalar, XDIM, YDIM>::y_type; \
 	protected: \
 		using typename Function<Scalar, XDIM, YDIM>::arg_type; \

 }

#endif // FUNCTION_HPP