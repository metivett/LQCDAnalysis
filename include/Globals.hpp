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
#include <memory>

#include "extEigen/Core"
#include "Eigen/LU"
#include "Utilities.hpp"
#include "IO.hpp"


/******************************************************************************
 *                             Compiler utilities                             *
 ******************************************************************************/

#ifdef __GNUC__
#define __unused __attibute__ ((unused))
#else
#define __unused
#endif

/******************************************************************************
*                                   Macros                                   *
******************************************************************************/

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE }

/******************************************************************************
*                                 Eigen                                      *
******************************************************************************/

BEGIN_NAMESPACE(LQCDA)

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

// ref
template<typename Derived>
using Ref = Eigen::Ref<Derived>;

template<typename Derived>
using ConstRef = Eigen::Ref<const Derived>;

// dense
template<typename Derived>
using DenseExpr = Eigen::DenseBase<Derived>;

// index type
typedef Matrix<double>::Index index_t;

// macros
#define FOR_MAT(m, i, j) \
for(index_t j = 0; j < (m).cols(); ++j) \
for(index_t i = 0; i < (m).rows(); ++i)

#define FOR_MAT_DIAG(m, i) \
for(index_t i = 0; i < std::min((m).rows(), (m).cols()); ++i)

#define FOR_VEC(v, i) \
for(index_t i = 0; i < (v).rows(); ++i)

#define FOR_ARRAY(a, i, j) \
for(index_t j = 0; j < (a).cols(); ++j) \
for(index_t i = 0; i < (a).rows(); ++i)

#define EIGEN_EXPR_CTOR(Ctor, Class, Base, ExprType) \
template<typename Derived> \
Ctor(const ExprType<Derived>& e): Base(e) {} \
template<typename Derived> \
Class& operator=(const ExprType<Derived>& e) \
{ \
	this->Base::operator=(e); \
	return *this; \
} \



/******************************************************************************
*                                 Enums                                      *
******************************************************************************/

enum Verbosity {
	SILENT = 0,
	NORMAL = 1,
	DEBUG = 2
};


/******************************************************************************
*                              Minimizer Types                               *
******************************************************************************/

struct MinimizerType
{
	virtual int id() const =0;
};

template<int i>
struct MinimizerID
: public MinimizerType
{
	static constexpr int ID = i;
	typedef MinimizerID<i> type;
	virtual int id() const override { return i; }
	constexpr operator int() { return ID; }
};

namespace MIN {
	typedef MinimizerID<0> DEFAULT_ID;
	typedef MinimizerID<1> MIGRAD_ID;

// static DEFAULT_ID DEFAULT;
// static MIGRAD_ID MIGRAD;
}

// enum class MinimizerType: int
// {
// 	DEFAULT,
// 	MIGRAD
// };

// bool operator< (const MinimizerType& t1, const MinimizerType& t2)
// {
// 	return static_cast<int>(t1) < static_cast<int>(t2);
// }
// bool operator> (const MinimizerType& t1, const MinimizerType& t2)
// {
// 	return static_cast<int>(t1) > static_cast<int>(t2);
// }
// bool operator== (const MinimizerType& t1, const MinimizerType& t2)
// {
// 	return static_cast<int>(t1) == static_cast<int>(t2);
// }

// class MinimizerID
// {
// private:
// 	const int id;

// public:
// 	static MinimizerID getNewId()
// 	{
// 		static int global_id = 0;
// 		return MinimizerID(global_id++);
// 	}

// 	bool operator<(const MinimizerID& t) const
// 	{
// 		return id < t.id;
// 	}
// 	bool operator>(const MinimizerID& t) const
// 	{
// 		return id > t.id;
// 	}
// 	bool operator==(const MinimizerID& t) const
// 	{
// 		return id == t.id;
// 	}

// private:
// 	MinimizerID(int i)
// 	: id{i}
// 	{}
// };

// namespace MinimizerType {
// 	static const MinimizerID DEFAULT = MinimizerID::getNewId();
// 	static const MinimizerID MIGRAD = MinimizerID::getNewId();
// }

END_NAMESPACE // LQCDA

#endif // GLOBALS_HPP