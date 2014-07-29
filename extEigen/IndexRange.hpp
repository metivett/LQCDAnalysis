/*
 * IndexRange.hpp
 *
 *  Created on: Jul 15, 2014
 *      Author: Thibaut Metivet
 */

#ifndef INDEX_RANGE_HPP
#define INDEX_RANGE_HPP

#include <Eigen/src/Core/util/Constants.h>

namespace Eigen {

template<int Step = 1>
class IndexRange
{
public:
	IndexRange(int begin, int end)
	: m_begin(begin)
	, m_end(end)
	{
		m_size = 1 + (m_end - m_begin)/Step;
	}

	int begin() const { return m_begin; }
	int end() const { return m_end; }
	int step() const { return Step; }
	int size() const {return m_size; }
	int operator[](int i) const { return m_begin + Step*i; }

private:
	int m_begin, m_end, m_size;
};

template<>
class IndexRange<Dynamic>
{
public:
	IndexRange(int begin, int end, int step)
	: m_begin(begin)
	, m_end(end)
	, m_step(step)
	{
		m_size = 1 + (m_end - m_begin)/m_step;
	}

	int begin() const { return m_begin; }
	int end() const { return m_end; }
	int step() const { return m_step; }
	int size() const {return m_size; }
	int operator[](int i) const { return m_begin + m_step*i; }

private:
	int m_begin, m_end, m_step, m_size;
};


/******************************************************************************
 *                              Helper functions                              *
 ******************************************************************************/

template<int Step>
IndexRange<Step> Range(int b, int e)
{
	return IndexRange<Step>(b, e);
}

inline IndexRange<> Range(int b, int e)
{
	return IndexRange<>(b, e);
}

inline IndexRange<Dynamic> Range(int b, int e, int s)
{
	return IndexRange<Dynamic>(b, e, s);
}

}

#endif // INDEX_RANGE_HPP
