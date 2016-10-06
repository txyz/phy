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

#include <iostream>
#include <stdexcept>

#include "Streamable.hpp"
#include "PhysicalUnit.h"

namespace phy {
  
/*
 * =====================================================================================
 *        Class:  PhysicalQuantity
 *  Description:  
 * =====================================================================================
 */
class PhysicalQuantity : public Streamable
{
  public:
    /* ====================  LIFECYCLE     ======================================= */
    PhysicalQuantity (double value,
                      const std::string &unit="",
                      const std::string &system=PhysicalUnit::SYSTEM_MKS)
        : m_value(value), m_unit(unit, system)
    {
    }                                           /* constructor      */

    PhysicalQuantity (const PhysicalQuantity &other)
        : m_value(other.m_value), m_unit(other.m_unit)
    {
    }                                           /* copy constructor */

    ~PhysicalQuantity () {};                    /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    double GetValue() const { return m_value * m_unit.GetScale(); }
    std::string GetUnit() const { return m_unit.GetName(); }

    double GetValue(const PhysicalUnit &unit) const {
        if (m_unit.GetName() != unit.GetName()) {
            std::stringstream ss;
            ss << "Can not convert '" << m_unit.GetName() << "' to '" << unit.GetName() << "'";
            throw std::invalid_argument(ss.str());
        }
        return GetValue() / unit.GetScale();
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

    /* ====================  METHODS       ======================================= */
    std::string ToString() const {
        std::stringstream ss;
        ss << GetValue() << " " << GetUnit();
        return ss.str();
    }

  protected:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */
    double                                                      m_value;
    PhysicalUnit                                                m_unit;

}; /* -----  end of class PhysicalQuantity  ----- */

}       /* -----  end of namespace phy  ----- */

#endif   /* ----- #ifndef PhysicalQuantity_INC  ----- */
