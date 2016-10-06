/*
 * =====================================================================================
 *
 *       Filename:  PhysicalUnit.h
 *
 *    Description:  Initial design of this class is targeting for convenience
 *                  instead of effciency
 *
 *        Version:  1.0
 *        Created:  09/30/2016 02:11:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongwei Yang (yhw1630), yhw1630@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  PHY_PhysicalUnit_INC
#define  PHY_PhysicalUnit_INC

#include "Streamable.hpp"
#include <map>

namespace phy {
/*
 * =====================================================================================
 *        Class:  PhysicalUnit
 *  Description:  
 * =====================================================================================
 */
class PhysicalUnit : public Streamable
{
  public:
    static const std::string SYSTEM_MKS;        // metre-kilogram-second system
    static const std::string SYSTEM_CGS;        // centimetre-gram-second system
    static const std::string SYSTEM_MTS;        // metre-tonne-second system

    /* ====================  LIFECYCLE     ======================================= */
    /* constructor      */
    PhysicalUnit (const std::string &name, const std::string &system=SYSTEM_MKS);
    PhysicalUnit (const PhysicalUnit &other);   /* copy constructor */
    ~PhysicalUnit ();                           /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    std::string GetName() const;
    double GetScale () const { return m_scale; }

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */
    PhysicalUnit& operator = (const PhysicalUnit &other);   /* assignment operator */

    /* ====================  METHODS       ======================================= */
    std::string ToString() const {return GetName();}

  protected:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  METHODS       ======================================= */
    void InitPhysicalUnitDict();
    void ParseUnit(const std::string &name, const std::string &system) const;

    /* ====================  DATA MEMBERS  ======================================= */
    std::string                                                     m_system;
    double                                                          m_scale;
    std::map<std::string, int>                                      m_unitDict;

    // Store base units [system of measure][name][scale, reference_unit_name]
    static std::map<std::string, std::map<std::string, std::pair<double, std::string> > >   m_baseUnitDict;

}; /* -----  end of class PhysicalUnit  ----- */

}       /* -----  end of namespace phy  ----- */

#endif   /* ----- #ifndef PhysicalUnit_INC  ----- */
