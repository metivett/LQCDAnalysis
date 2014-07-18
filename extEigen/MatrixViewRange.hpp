/*
 * MatrixViewRange.hpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Thibaut Metivet
 */

#ifndef MATRIX_VIEW_HPP
#define MATRIX_VIEW_HPP

namespace Eigen {

namespace internal {
template<typename XprType, int ViewRows, int ViewCols, int RowStride, int ColStride>
struct traits<MatrixViewRange<XprType, ViewRows, ViewCols, RowStride, ColStride> > : traits<XprType>
{
	typedef typename traits<XprType>::Scalar Scalar;
	typedef typename traits<XprType>::StorageKind StorageKind;
	typedef typename traits<XprType>::XprKind XprKind;
	typedef typename nested<XprType>::type XprTypeNested;
	typedef typename remove_reference<XprTypeNested>::type _XprTypeNested;
	enum{
		MatrixRows = traits<XprType>::RowsAtCompileTime,
		MatrixCols = traits<XprType>::ColsAtCompileTime,
		RowsAtCompileTime = MatrixRows == 0 ? 0 : ViewRows,
		ColsAtCompileTime = MatrixCols == 0 ? 0 : ViewCols,
		MaxRowsAtCompileTime = ViewRows==0 ? 0
		: RowsAtCompileTime != Dynamic ? int(RowsAtCompileTime)
		: int(traits<XprType>::MaxRowsAtCompileTime),
		MaxColsAtCompileTime = ViewCols==0 ? 0
		: ColsAtCompileTime != Dynamic ? int(ColsAtCompileTime)
		: int(traits<XprType>::MaxColsAtCompileTime),
		XprTypeIsRowMajor = (int(traits<XprType>::Flags)&RowMajorBit) != 0,
		IsRowMajor = (MaxRowsAtCompileTime==1&&MaxColsAtCompileTime!=1) ? 1
		: (MaxColsAtCompileTime==1&&MaxRowsAtCompileTime!=1) ? 0
		: XprTypeIsRowMajor,
		HasSameStorageOrderAsXprType = (IsRowMajor == XprTypeIsRowMajor),
		InnerSize = IsRowMajor ? int(ColsAtCompileTime) : int(RowsAtCompileTime),
		InnerStrideAtCompileTime = HasSameStorageOrderAsXprType
		? int(inner_stride_at_compile_time<XprType>::ret)
		: int(outer_stride_at_compile_time<XprType>::ret),
		OuterStrideAtCompileTime = HasSameStorageOrderAsXprType
		? int(outer_stride_at_compile_time<XprType>::ret)
		: int(inner_stride_at_compile_time<XprType>::ret),
		MaskPacketAccessBit = (InnerSize == Dynamic || (InnerSize % packet_traits<Scalar>::size) == 0)
		&& (InnerStrideAtCompileTime == 1)
		? PacketAccessBit : 0,
		MaskAlignedBit = ((OuterStrideAtCompileTime!=Dynamic) && (((OuterStrideAtCompileTime * int(sizeof(Scalar))) % 16) == 0)) ? AlignedBit : 0,
		FlagsLinearAccessBit = (RowsAtCompileTime == 1 || ColsAtCompileTime == 1) ? LinearAccessBit : 0,
		FlagsLvalueBit = is_lvalue<XprType>::value ? LvalueBit : 0,
		FlagsRowMajorBit = IsRowMajor ? RowMajorBit : 0,
		Flags0 = traits<XprType>::Flags & ( (HereditaryBits & ~RowMajorBit) |
			DirectAccessBit |
			MaskPacketAccessBit |
			MaskAlignedBit),
		Flags = Flags0 | FlagsLinearAccessBit | FlagsLvalueBit | FlagsRowMajorBit
	};
};

template
< typename XprType
, int ViewRows=Dynamic
, int ViewCols=Dynamic
, int RowStride=Dynamic
, int ColStride=Dynamic
, bool HasDirectAccess = internal::has_direct_access<XprType>::ret
>
class MatrixViewRangeImpl_dense;

}

template<typename XprType, int ViewRows, int ViewCols, int RowStride, int ColStride, typename StorageKind>
class MatrixViewRangeImpl;

template<typename XprType, int ViewRows, int ViewCols, int RowStride, int ColStride>
class MatrixViewRange
: public MatrixViewRangeImpl<XprType, ViewRows, ViewCols, RowStride, ColStride, typename internal::traits<XprType>::StorageKind>
{
	typedef MatrixViewRangeImpl<XprType, ViewRows, ViewCols, RowStride, ColStride, typename internal::traits<XprType>::StorageKind> Impl;

	public:
		typedef Impl Base;
		EIGEN_GENERIC_PUBLIC_INTERFACE(MatrixViewRange)
    	EIGEN_INHERIT_ASSIGNMENT_OPERATORS(MatrixViewRange)

    	// // Column or Row constructor
    	// inline MatrixViewRange(XprType& xpr, Index i)
    	// : Impl(xpr, i)
    	// {
    	// 	eigen_assert( (i>=0) && (
    	// 		((ViewRows==1) && (ViewCols==XprType::ColsAtCompileTime) && i<xpr.rows())
    	// 		||((ViewRows==XprType::RowsAtCompileTime) && (ViewCols==1) && i<xpr.cols())));
    	// }

    	// Fixed-size constructor
    	inline MatrixViewRange(XprType& xpr, Index a_startRow, Index a_startCol)
    	: Impl(xpr, a_startRow, a_startCol)
    	{
    		EIGEN_STATIC_ASSERT(RowsAtCompileTime!=Dynamic && ColsAtCompileTime!=Dynamic 
    			&& RowStride!=Dynamic && ColStride!=Dynamic
    			,THIS_METHOD_IS_ONLY_FOR_FIXED_SIZE)
    		eigen_assert(a_startRow >= 0 && ViewRows >= 1 && a_startRow + (ViewRows - 1)*RowStride < xpr.rows()
    			&& a_startCol >= 0 && ViewCols >= 1 && a_startCol + (ViewCols - 1)*ColStride < xpr.cols());
    	}

    	// Dynamic-size, fixed-strides constructor
    	inline MatrixViewRange(XprType& xpr,
    		Index a_startRow, Index a_startCol,
    		Index viewRows, Index viewCols)
    	: Impl(xpr, a_startRow, a_startCol, viewRows, viewCols)
    	{
    		EIGEN_STATIC_ASSERT(RowStride!=Dynamic && ColStride!=Dynamic, THIS_METHOD_IS_ONLY_FOR_FIXED_SIZE)
    		eigen_assert((RowsAtCompileTime==Dynamic || RowsAtCompileTime==viewRows)
    			&& (ColsAtCompileTime==Dynamic || ColsAtCompileTime==viewCols));
    		eigen_assert(a_startRow >= 0 && viewRows >= 0 && a_startRow + (viewRows - 1)*RowStride < xpr.rows()
    			&& a_startCol >= 0 && viewCols >= 0 && a_startCol + (viewCols - 1)*ColStride < xpr.cols());
    	}

    	// Dynamic-size, dynamic-strides constructor
    	inline MatrixViewRange(XprType& xpr,
    		Index a_startRow, Index a_startCol,
    		Index viewRows, Index viewCols,
    		Index rowStride, Index colStride)
    	: Impl(xpr, a_startRow, a_startCol, viewRows, viewCols, rowStride, colStride)
    	{
    		eigen_assert((RowsAtCompileTime==Dynamic || RowsAtCompileTime==viewRows)
    			&& (ColsAtCompileTime==Dynamic || ColsAtCompileTime==viewCols)
    			&& (RowStride==Dynamic || RowStride==rowStride)
    			&& (ColStride==Dynamic || ColStride==colStride));
    		eigen_assert(a_startRow >= 0 && viewRows >= 0 && a_startRow + (viewRows - 1)*rowStride < xpr.rows()
    			&& a_startCol >= 0 && viewCols >= 0 && a_startCol + (viewCols - 1)*colStride < xpr.cols());
    	}
};

template<typename XprType, int ViewRows, int ViewCols, int RowStride, int ColStride>
class MatrixViewRangeImpl<XprType, ViewRows, ViewCols, RowStride, ColStride, Dense>
: public internal::MatrixViewRangeImpl_dense<XprType, ViewRows, ViewCols, RowStride, ColStride>
{
	typedef internal::MatrixViewRangeImpl_dense<XprType, ViewRows, ViewCols, RowStride, ColStride> Impl;
	typedef typename XprType::Index Index;

public:
	typedef Impl Base;

	EIGEN_INHERIT_ASSIGNMENT_OPERATORS(MatrixViewRangeImpl)
	// inline MatrixViewRangeImpl(XprType& xpr, Index i) : Impl(xpr, i) {}
	inline MatrixViewRangeImpl(XprType& xpr, Index a_startRow, Index a_startCol)
	: Impl(xpr, a_startRow, a_startCol)
	{}
	inline MatrixViewRangeImpl(XprType& xpr, Index a_startRow, Index a_startCol, Index viewRows, Index viewCols)
	: Impl(xpr, a_startRow, a_startCol, viewRows, viewCols)
	{}
	inline MatrixViewRangeImpl(XprType& xpr,
		Index a_startRow, Index a_startCol,
		Index viewRows, Index viewCols,
		Index rowStride, Index colStride)
	: Impl(xpr, a_startRow, a_startCol, viewRows, viewCols, rowStride, colStride)
	{}
};

namespace internal {

template<typename XprType, int ViewRows, int ViewCols, int RowStride, int ColStride, bool HasDirectAccess>
class MatrixViewRangeImpl_dense
: public internal::dense_xpr_base<MatrixViewRange<XprType, ViewRows, ViewCols, RowStride, ColStride> >::type
{
	typedef MatrixViewRange<XprType, ViewRows, ViewCols, RowStride, ColStride> MatrixViewRangeType;
public:

	typedef typename internal::dense_xpr_base<MatrixViewRangeType>::type Base;
	EIGEN_DENSE_PUBLIC_INTERFACE(MatrixViewRangeType)

	using Base::rowStride;
    using Base::colStride;

	EIGEN_INHERIT_ASSIGNMENT_OPERATORS(MatrixViewRangeImpl_dense)

	// // Column or Row constructor
	// inline MatrixViewRangeImpl_dense(XprType& xpr, Index i)
	// : m_xpr(xpr),
 //    // It is a row if and only if MatrixViewRangeRows==1 and MatrixViewRangeCols==XprType::ColsAtCompileTime,
 //    // and it is a column if and only if MatrixViewRangeRows==XprType::RowsAtCompileTime and MatrixViewRangeCols==1,
 //    // all other cases are invalid.
 //    // The case a 1x1 matrix seems ambiguous, but the result is the same anyway.
	// m_startRow( (ViewRows==1) && (ViewCols==XprType::ColsAtCompileTime) ? i : 0),
	// m_startCol( (ViewRows==XprType::RowsAtCompileTime) && (ViewCols==1) ? i : 0),
	// m_viewRows(ViewRows==1 ? 1 : xpr.rows()),
	// m_viewCols(ViewCols==1 ? 1 : xpr.cols())
	// {}

/** Fixed-size constructor
  */
	inline MatrixViewRangeImpl_dense(XprType& xpr, Index a_startRow, Index a_startCol)
	: m_xpr(xpr)
	, m_startRow(a_startRow)
	, m_startCol(a_startCol)
	, m_viewRows(ViewRows)
	, m_viewCols(ViewCols)
	, m_rowStride(RowStride)
	, m_colStride(ColStride)
	{}

/** Dynamic-size, fixed-strides constructor
  */
	inline MatrixViewRangeImpl_dense(XprType& xpr,
		Index a_startRow, Index a_startCol,
		Index viewRows, Index viewCols)
	: m_xpr(xpr)
	, m_startRow(a_startRow)
	, m_startCol(a_startCol)
	, m_viewRows(viewRows)
	, m_viewCols(viewCols)
	, m_rowStride(RowStride)
	, m_colStride(ColStride)
	{}

	/** Dynamic-size, dynamic-strides constructor
  */
	inline MatrixViewRangeImpl_dense(XprType& xpr,
		Index a_startRow, Index a_startCol,
		Index viewRows, Index viewCols,
		Index rowStride, Index colStride)
	: m_xpr(xpr)
	, m_startRow(a_startRow)
	, m_startCol(a_startCol)
	, m_viewRows(viewRows)
	, m_viewCols(viewCols)
	, m_rowStride(rowStride)
	, m_colStride(colStride)
	{}

	inline Index rows() const { return m_viewRows.value(); }
	inline Index cols() const { return m_viewCols.value(); }

	inline Index innerStride() const
	{
		// return (StrideType::InnerStrideAtCompileTime != 0 ? m_stride.inner() : 1) * m_xpr.innerStride();
		return (int(Flags)&RowMajorBit ? m_colStride.value() : m_rowStride.value()) * m_xpr.innerStride();
	}

	inline Index outerStride() const
	{
		// return (StrideType::OuterStrideAtCompileTime != 0 ? m_stride.outer()
		// : IsVectorAtCompileTime ? this->size()
		// : int(Flags)&RowMajorBit ? this->cols()
		// : this->rows()) * m_xpr.outerStride();
		return (int(Flags)&RowMajorBit ? m_rowStride.value() : m_colStride.value()) * m_xpr.outerStride();
	}

	inline Scalar& coeffRef(Index rowId, Index colId)
	{
		EIGEN_STATIC_ASSERT_LVALUE(XprType)
		return m_xpr.const_cast_derived()
		.coeffRef(rowId*rowStride() + m_startRow.value(), colId*colStride() + m_startCol.value());
	}

	inline const Scalar& coeffRef(Index rowId, Index colId) const
	{
		return m_xpr.derived()
		.coeffRef(rowId*rowStride() + m_startRow.value(), colId*colStride() + m_startCol.value());
	}

	EIGEN_STRONG_INLINE const CoeffReturnType coeff(Index rowId, Index colId) const
	{
		return m_xpr.coeff(rowId*rowStride() + m_startRow.value(), colId*colStride() + m_startCol.value());
	}

	inline Scalar& coeffRef(Index index)
	{
		EIGEN_STATIC_ASSERT_LVALUE(XprType)
		return m_xpr.const_cast_derived()
		.coeffRef(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index*rowStride()),
			m_startCol.value() + (RowsAtCompileTime == 1 ? index*colStride() : 0));
	}

	inline const Scalar& coeffRef(Index index) const
	{
		return m_xpr.const_cast_derived()
		.coeffRef(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index*rowStride()),
			m_startCol.value() + (RowsAtCompileTime == 1 ? index*colStride() : 0));
	}

	inline const CoeffReturnType coeff(Index index) const
	{
		return m_xpr
		.coeff(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index*rowStride()),
			m_startCol.value() + (RowsAtCompileTime == 1 ? index*colStride() : 0));
	}

	template<int LoadMode>
	inline PacketScalar packet(Index rowId, Index colId) const
	{
		return m_xpr.template packet<Unaligned>
		(rowId*rowStride() + m_startRow.value(), colId*colStride() + m_startCol.value());
	}

	template<int LoadMode>
	inline void writePacket(Index rowId, Index colId, const PacketScalar& val)
	{
		m_xpr.const_cast_derived().template writePacket<Unaligned>
		(rowId*rowStride() + m_startRow.value(), colId*colStride() + m_startCol.value(), val);
	}

	template<int LoadMode>
	inline PacketScalar packet(Index index) const
	{
		return m_xpr.template packet<Unaligned>
		(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index*rowStride()),
			m_startCol.value() + (RowsAtCompileTime == 1 ? index*colStride() : 0));
	}

	template<int LoadMode>
	inline void writePacket(Index index, const PacketScalar& val)
	{
		m_xpr.const_cast_derived().template writePacket<Unaligned>
		(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index*rowStride()),
			m_startCol.value() + (RowsAtCompileTime == 1 ? index*colStride() : 0), val);
	}

	const typename internal::remove_all<typename XprType::Nested>::type& nestedExpression() const 
	{ 
		return m_xpr; 
	}

	Index startRow() const 
	{ 
		return m_startRow.value(); 
	}

	Index startCol() const 
	{ 
		return m_startCol.value(); 
	}

protected:

	const typename XprType::Nested m_xpr;
	const internal::variable_if_dynamic<Index, XprType::RowsAtCompileTime == 1 ? 0 : Dynamic> m_startRow;
	const internal::variable_if_dynamic<Index, XprType::ColsAtCompileTime == 1 ? 0 : Dynamic> m_startCol;
	const internal::variable_if_dynamic<Index, RowsAtCompileTime> m_viewRows;
	const internal::variable_if_dynamic<Index, ColsAtCompileTime> m_viewCols;
	const internal::variable_if_dynamic<Index, RowStride> m_rowStride;
	const internal::variable_if_dynamic<Index, ColStride> m_colStride;
};

template<typename XprType, int ViewRows, int ViewCols, int RowStride, int ColStride>
class MatrixViewRangeImpl_dense<XprType,ViewRows,ViewCols, RowStride, ColStride, true>
: public MapBase<MatrixViewRange<XprType, ViewRows, ViewCols, RowStride, ColStride> >
{
	typedef MatrixViewRange<XprType, ViewRows, ViewCols, RowStride, ColStride> MatrixViewRangeType;
public:

	typedef MapBase<MatrixViewRangeType> Base;
	EIGEN_DENSE_PUBLIC_INTERFACE(MatrixViewRangeType)
	EIGEN_INHERIT_ASSIGNMENT_OPERATORS(MatrixViewRangeImpl_dense)

// /** Column or Row constructor
//   */
// inline MatrixViewRangeImpl_dense(XprType& xpr, Index i)
//   : Base(internal::const_cast_ptr(&xpr.coeffRef(
//           (MatrixViewRangeRows==1) && (MatrixViewRangeCols==XprType::ColsAtCompileTime) ? i : 0,
//           (MatrixViewRangeRows==XprType::RowsAtCompileTime) && (MatrixViewRangeCols==1) ? i : 0)),
//          MatrixViewRangeRows==1 ? 1 : xpr.rows(),
//          MatrixViewRangeCols==1 ? 1 : xpr.cols()),
//     m_xpr(xpr)
// {
//   init();
// }

/** Fixed-size constructor
  */
	inline MatrixViewRangeImpl_dense(XprType& xpr, Index a_startRow, Index a_startCol)
	: Base(internal::const_cast_ptr(&xpr.coeffRef(a_startRow,a_startCol)))
	, m_xpr(xpr)
	, m_rowStride(RowStride)
	, m_colStride(ColStride)
	{
		init();
	}

/** Dynamic-size constructor
  */
	inline MatrixViewRangeImpl_dense(XprType& xpr,
		Index a_startRow, Index a_startCol,
		Index viewRows, Index viewCols)
	: Base(internal::const_cast_ptr(&xpr.coeffRef(a_startRow,a_startCol)), viewRows, viewCols)
	, m_xpr(xpr)
	, m_rowStride(RowStride)
	, m_colStride(ColStride)
	{
		init();
	}

	inline MatrixViewRangeImpl_dense(XprType& xpr,
		Index a_startRow, Index a_startCol,
		Index viewRows, Index viewCols,
		Index rowStride, Index colStride)
	: Base(internal::const_cast_ptr(&xpr.coeffRef(a_startRow,a_startCol)), viewRows, viewCols)
	, m_xpr(xpr)
	, m_rowStride(rowStride)
	, m_colStride(colStride)
	{
		init();
	}

	const typename internal::remove_all<typename XprType::Nested>::type& nestedExpression() const 
	{ 
		return m_xpr; 
	}

/** \sa MapBase::innerStride() */
	inline Index innerStride() const
	{
		// return internal::traits<MatrixViewRangeType>::HasSameStorageOrderAsXprType
		// ? m_xpr.innerStride() * (int(Flags)&RowMajorBit ? m_colStride.value() : m_rowStride.value())
		// : m_xpr.outerStride() * (int(Flags)&RowMajorBit ? m_rowStride.value() : m_colStride.value());
		return m_innerStride;
	}

/** \sa MapBase::outerStride() */
	inline Index outerStride() const
	{
		return m_outerStride;
	}

#ifndef EIGEN_PARSED_BY_DOXYGEN
/** \internal used by allowAligned() */
	inline MatrixViewRangeImpl_dense(XprType& xpr, const Scalar* data, Index viewRows, Index viewCols)
	: Base(data, viewRows, viewCols), m_xpr(xpr)
	{
		init();
	}
#endif

protected:
	void init()
	{
		m_innerStride = (internal::traits<MatrixViewRangeType>::HasSameStorageOrderAsXprType
						? m_xpr.innerStride()
						: m_xpr.outerStride()) * (int(Flags)&RowMajorBit ? m_colStride.value() : m_rowStride.value());
		m_outerStride = (internal::traits<MatrixViewRangeType>::HasSameStorageOrderAsXprType
						? m_xpr.outerStride()
						: m_xpr.innerStride()) * (int(Flags)&RowMajorBit ? m_rowStride.value() : m_colStride.value());
	}

	typename XprType::Nested m_xpr;
	const internal::variable_if_dynamic<Index, RowStride> m_rowStride;
	const internal::variable_if_dynamic<Index, ColStride> m_colStride;
	Index m_innerStride;
	Index m_outerStride;
};

}

}

#endif // MATRIX_VIEW_HPP
