/*
 * MatrixViewMethods.hpp
 *
 *  Created on: Jul 16, 2014
 *      Author: Thibaut Metivet
 */

// template<int ViewRows, int ViewCols, int RowStride, int ColStride>
// inline MatrixViewRange<Derived, ViewRows, ViewCols, RowStride, ColStride> view(const IndexRange<RowStride>& r1, const IndexRange<ColStride>& r2)
// {
// 	return MatrixViewRange<Derived, ViewRows, ViewCols, RowStride, ColStride>(derived(), r1.begin(), r2.begin());
// }

// template<int ViewRows, int ViewCols, int RowStride, int ColStride>
// inline MatrixViewRange<Derived, ViewRows, ViewCols, RowStride, ColStride> view(const IndexRange<RowStride>& r1, const IndexRange<ColStride>& r2)
// {
// 	return MatrixViewRange<Derived, ViewRows, ViewCols, RowStride, ColStride>(derived(), r1.begin(), r2.begin(), r1.size(), r2.size());
// }

template<int ViewRows, int ViewCols, int RowStride, int ColStride>
inline MatrixViewRange<Derived, ViewRows, ViewCols, RowStride, ColStride> view(const IndexRange<RowStride>& r1, const IndexRange<ColStride>& r2)
{
	return MatrixViewRange<Derived, ViewRows, ViewCols, RowStride, ColStride>(derived(), r1.begin(), r2.begin(), r1.size(), r2.size(), r1.step(), r2.step());
}

template<int RowStride, int ColStride>
inline MatrixViewRange<Derived, Dynamic, Dynamic, RowStride, ColStride> view(const IndexRange<RowStride>& r1, const IndexRange<ColStride>& r2)
{
	return MatrixViewRange<Derived, Dynamic, Dynamic, RowStride, ColStride>(derived(), r1.begin(), r2.begin(), r1.size(), r2.size(), r1.step(), r2.step());
}

template<int ColStride>
inline MatrixViewRange<Derived, 1, Dynamic, 1, ColStride> view(Index i, const IndexRange<ColStride>& r2)
{
	return MatrixViewRange<Derived, 1, Dynamic, 1, ColStride>(derived(), i, r2.begin(), 1, r2.size(), 1, r2.step());
}

template<int RowStride>
inline MatrixViewRange<Derived, Dynamic, 1, RowStride, 1> view(const IndexRange<RowStride>& r1, Index j)
{
	return MatrixViewRange<Derived, Dynamic, 1, RowStride, 1>(derived(), r1.begin(), j, r1.size(), 1, r1.step(), 1);
}