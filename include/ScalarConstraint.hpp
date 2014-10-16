/*
 * ScalarConstraint.hpp
 *
 *  Created on: Sep 03, 2014
 *      Author: Thibaut Metivet
 */

#ifndef SCALAR_CONSTRAINT_HPP
#define SCALAR_CONSTRAINT_HPP

#include "Globals.hpp"

BEGIN_NAMESPACE(LQCDA)

template<typename T>
class ScalarConstraint
{
public:
    // Typedefs
    typedef T Scalar;

public:
    // Constructors / Destructor
    ScalarConstraint() = default;

    ~ScalarConstraint() = default;

    // Constraints modifiers
    void setBounds(T a, T b) { setLowerBound(a); setUpperBound(b); }
    void setLowerBound(T a) { m_HasLowerBound = true; m_LowerBound = a; }
    void setUpperBound(T b) { m_HasUpperBound = true; m_UpperBound = b; }
    void setNone() { clear(); }

    // Constraints access
    bool hasBounds() const { return hasLowerBound() || hasUpperBound(); }
    bool hasLowerBound() const { return m_HasLowerBound; }
    T lowerBound() const { return m_LowerBound; }
    bool hasUpperBound() const { return m_HasUpperBound; }
    T upperBound() const { return m_UpperBound; }

private:
    void clear();

private:
    bool m_HasLowerBound {false};
    T m_LowerBound;
    bool m_HasUpperBound {false};
    T m_UpperBound;
};

template<typename T>
void ScalarConstraint<T>::clear()
{
    m_HasLowerBound = false;
    m_HasUpperBound = false;
}

END_NAMESPACE

#endif // SCALAR_CONSTRAINT_HPP