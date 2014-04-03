/*
 * Globals.hpp
 *
 *  Created on: Jan 28, 2014
 *      Author: Thibaut Metivet
 */

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

 #include <vector>
 #include <array>
 #include <complex>
 #include <string>
 #include <iostream>
 #include <algorithm>

 #include "Eigen/Core"
 #include "Eigen/LU"
 #include "Utilities.hpp"
 #include "IO.hpp"

 namespace LQCDA {

/******************************************************************************
 *                             Compiler utilities                             *
 ******************************************************************************/

 	#ifdef __GNUC__
 	#define __unused __attibute__ ((unused))
 	#else
 	#define __unused
 	#endif

/******************************************************************************
 *                                 Eigen                                      *
 ******************************************************************************/

 	const int Dynamic = Eigen::Dynamic;

 	// array
 	template<typename Derived>
 	using ArrayExpr = Eigen::ArrayBase<Derived>;

 	template<typename T, int NRows = Dynamic, int NCols = Dynamic>
 	using Array = Eigen::Array<T, NRows, NCols>;

 	// vector
 	template<typename T, int Size = Dynamic>
 	using Vector = Eigen::Matrix<T, Size, 1>;

 	template<typename T, int Size = Dynamic>
 	using RowVector = Eigen::Matrix<T, 1, Size>;

 	// matrix
 	template<typename Derived>
 	using MatrixExpr = Eigen::MatrixBase<Derived>;

 	template<typename T, int NRows = Dynamic, int NCols = Dynamic>
 	using Matrix = Eigen::Matrix<T, NRows, NCols>;

 	// block
 	template<typename Derived>
 	using Block = Eigen::Block<Derived>;

 	template<typename Derived>
 	using ConstBlock = Eigen::Block<const Derived>;

 	// row
 	template<typename Derived>
 	using Row = typename Derived::RowXpr;

 	// col
 	template<typename Derived>
 	using Col = typename Derived::ColXpr;

 	// map
 	template<typename Derived>
 	using Map = Eigen::Map<Derived>;

 	template<typename Derived>
 	using ConstMap = Eigen::Map<const Derived>;

 	// index type
 	typedef Matrix<double>::Index index_t;

 	// macros
 	#define FOR_MAT(m, i, j) \
 	for(index_t j = 0; j < m.cols(); ++j) \
 	for(index_t i = 0; i < m.rows(); ++i)

 	#define FOR_VEC(v, i) \
 	for(index_t i = 0; i < v.rows(); ++i)

 	#define FOR_ARRAY(a, i) \
 	for(index_t i = 0; i < a.rows(); ++i)


/******************************************************************************
 *                                 Enums                                      *
 ******************************************************************************/
	
	enum Verbosity {
		SILENT,
		NORMAL,
		DEBUG
	};


/******************************************************************************
 *                              Minimizer Types                               *
 ******************************************************************************/

 	class MinimizerID
 	{
 	private:
 		const int id;

 	public:
 		static MinimizerID getNewId()
 		{
 			static int global_id = 0;
 			return MinimizerID(global_id++);
 		}

 		bool operator<(const MinimizerID& t) const
	 	{
	 		return id < t.id;
	 	}
	 	bool operator>(const MinimizerID& t) const
	 	{
	 		return id > t.id;
	 	}
	 	bool operator==(const MinimizerID& t) const
	 	{
	 		return id == t.id;
	 	}

 	private:
 		MinimizerID(int i)
 		: id{i}
 		{}
 	};

 	namespace MinimizerType {
 		static const MinimizerID DEFAULT = MinimizerID::getNewId();
 		static const MinimizerID MIGRAD = MinimizerID::getNewId();
 	}

 }

#endif // GLOBALS_HPP