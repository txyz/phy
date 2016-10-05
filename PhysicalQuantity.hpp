/*
 * =====================================================================================
 *
 *       Filename:  PhysicalQuantity.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/30/2016 02:06:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongwei Yang (yhw1630), yhw1630@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  PHY_PhysicalQuantity_INC
#define  PHY_PhysicalQuantity_INC

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "PhysicalUnit.h"

namespace phy {

/*
 * =====================================================================================
 *        Class:  PhysicalQuantity
 *  Description:  
 * =====================================================================================
 */
template <typename T>
class PhysicalQuantity
{
  public:
    /* ====================  LIFECYCLE     ======================================= */
    PhysicalQuantity (T value, const std::string &unit="")
        : m_value(value), m_unit(unit)
    {
    }                                           /* constructor      */

    PhysicalQuantity (const PhysicalQuantity &other)
        : m_value(other.m_value),
        m_unit(other.m_unit)
    {
    }                                           /* copy constructor */

    ~PhysicalQuantity () {};                    /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    T GetValue() const {return m_value;}
    std::string GetUnit() const {return m_unit.GetName();}
    T GetValue(const PhysicalUnit &unit) const {

        if (m_unit.GetName() == unit.GetName()) return m_value;

        if (m_unit.GetBaseName() != unit.GetBaseName()) {
            std::stringstream ss;
            ss << "Can not convert '" << m_unit.GetName() << "' to '" << unit.GetName() << "'";
            throw std::invalid_argument(ss.str());
        }
        return m_value * m_unit.GetScale() / unit.GetScale();
    }

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    PhysicalQuantity operator + (const PhysicalQuantity &other) const {
        PhysicalQuantity result(*this);
        return result += other;
    }                                           /* addition operator */

    PhysicalQuantity operator - (const PhysicalQuantity &other) const {
        PhysicalQuantity result(*this);
        return result -= other;
    }                                           /* subtraction operator */

    PhysicalQuantity& operator += (const PhysicalQuantity &other) {
        m_value += other.GetValue(m_unit);
        return *this;
    }                                           /* addition assignment operator */

    PhysicalQuantity& operator -= (const PhysicalQuantity &other) {
        m_value -= other.GetValue(m_unit);
        return *this;
    }                                           /* subtraction assignment operator */

    PhysicalQuantity& operator = (const PhysicalQuantity &other) {
        if (this != &other) {
            m_value = other.m_value;
            m_unit = other.m_unit;
        }
        return *this;
    }                                           /* assignment operator */

  protected:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */
    T                   m_value;
    PhysicalUnit        m_unit;

}; /* -----  end of class PhysicalQuantity  ----- */

template <typename T>
std::ostream& operator << (std::ostream &os, const PhysicalQuantity<T> &obj)
{
    os << obj.GetValue() << " " << obj.GetUnit();
    return os;
}       /* -----  end of function operator <<  ----- */

}       /* -----  end of namespace phy  ----- */

#endif   /* ----- #ifndef PhysicalQuantity_INC  ----- */
