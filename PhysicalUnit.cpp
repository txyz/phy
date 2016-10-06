/*
 * =====================================================================================
 *
 *       Filename:  PhysicalUnit.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/30/2016 02:12:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongwei Yang (yhw1630), yhw163@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "PhysicalUnit.h"

#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <vector>

#include "Utilities.h"

namespace phy {

std::map<std::string, std::map<std::string, std::pair<double, std::string> > > PhysicalUnit::m_baseUnitDict;
const std::string PhysicalUnit::SYSTEM_MKS("mks");        // metre-kilogram-second system
const std::string PhysicalUnit::SYSTEM_CGS("cgs");        // centimetre-gram-second system
const std::string PhysicalUnit::SYSTEM_MTS("mts");        // metre-tonne-second system

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PhysicalUnit
 *      Method:  PhysicalUnit
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
PhysicalUnit::PhysicalUnit (const std::string &name, const std::string &system)
{
    InitPhysicalUnitDict();
    if (m_baseUnitDict.find(system) == m_baseUnitDict.end()) {
        std::stringstream ss;
        ss << "Unknown system of measure: " << system;
        throw std::invalid_argument(ss.str());
    } 

    if (system == SYSTEM_MKS) {
        m_unitDict["m"] = 0;
        m_unitDict["kg"] = 0;
        m_unitDict["s"] = 0;
        m_unitDict["A"] = 0;
        m_unitDict["K"] = 0;
        m_unitDict["mol"] = 0;
        m_unitDict["cd"] = 0;
    } else {
        std::stringstream ss;
        ss << system << " system not supported yet";
        throw std::runtime_error(ss.str());
    }

    // Composite unit parser, currently support '*', '/', '^', do not support '(', ')'
    m_scale = 1;
    std::string unitName, baseName;
    // '*' first
    std::vector<std::string> splitted1 = utl::split(name, '*');
    std::vector<std::string>::iterator it1;
    bool isDenominator;
    for (it1 = splitted1.begin(); it1 != splitted1.end(); ++it1) {
        isDenominator = false;
        // Now '/'
        utl::trim(*it1);
        if (it1->empty()) {
            // Two '*' separated by empty space
            std::stringstream ss;
            ss << "Duplicated '*' sign in: " << name;
            throw std::invalid_argument(ss.str());
        }
        if (*(it1->begin()) == '/' || *(it1->rbegin()) == '/') {
            // Unit can not start or end with '/'
            std::stringstream ss;
            ss << "Invalid format near: " << *it1;
            throw std::invalid_argument(ss.str());
        }
        std::vector<std::string> splitted2 = utl::split(*it1, '/');
        std::vector<std::string>::iterator it2;
        for (it2 = splitted2.begin(); it2 != splitted2.end(); ++it2) {
            utl::trim(*it2);
            std::vector<std::string> splitted3 = utl::split(*it2, '^');
            int order = 0;
            if (splitted3.empty()) {
                // Two '/' separated by empty space
                std::stringstream ss;
                ss << "Duplicated '/' sign in: " << *it1;
                throw std::invalid_argument(ss.str());
            }
            switch (splitted3.size()) {
                case 1:
                    order = 1;
                    break;
                case 2:
                    order = atoi(splitted3[1].c_str());
                    break;
                default:
                    std::stringstream ss;
                    ss << "Unknown unit: " << *it2;
                    throw std::invalid_argument(ss.str());
            }
            if (order != 0) {
                std::string unitName = splitted3[0];
                if (m_baseUnitDict[system].find(unitName) == m_baseUnitDict[system].end()) {
                    std::stringstream ss;
                    ss << "Unknown unit: " << name;
                    throw std::invalid_argument(ss.str());
                }

                std::pair<double, std::string> info = m_baseUnitDict[system][unitName];
                baseName = info.second;

                if (m_unitDict.find(baseName) != m_unitDict.end()) {
                    m_unitDict[baseName] = 0;
                }

                if (isDenominator) {
                    m_unitDict[baseName] -= order;
                    m_scale /= info.first;
                } else {
                    m_unitDict[baseName] += order;
                    m_scale *= info.first;
                }
            }
            // Only after first element, it becomes denominator
            isDenominator = true;
        }

    }

    m_system = system;

}  /* -----  end of method PhysicalUnit::PhysicalUnit  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PhysicalUnit
 *      Method:  PhysicalUnit
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
PhysicalUnit::PhysicalUnit (const PhysicalUnit &other):
    m_system(other.m_system),
    m_scale(other.m_scale),
    m_unitDict(other.m_unitDict)
{
}  /* -----  end of method PhysicalUnit::PhysicalUnit  (copy constructor)  ----- */

/*
 *----------------------------------------------------------------:q----------------------
 *       Class:  PhysicalUnit
 *      Method:  ~PhysicalUnit
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
PhysicalUnit::~PhysicalUnit ()
{
}  /* -----  end of method PhysicalUnit::~PhysicalUnit  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PhysicalUnit
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
PhysicalUnit&
PhysicalUnit::operator = (const PhysicalUnit &other)
{
    if (this != &other) {
        m_system = other.m_system;
        m_scale = other.m_scale;
        m_unitDict = other.m_unitDict;
    }
    return *this;
}  /* -----  end of method PhysicalUnit::operator =  (assignment operator)  ----- */

void PhysicalUnit::InitPhysicalUnitDict ()
{
    if (!m_baseUnitDict.empty()) return;

    m_baseUnitDict[SYSTEM_MKS]["nm"] = std::pair<double, std::string>(1e-9, "m");
    m_baseUnitDict[SYSTEM_MKS]["um"] = std::pair<double, std::string>(1e-6, "m");
    m_baseUnitDict[SYSTEM_MKS]["mm"] = std::pair<double, std::string>(1e-3, "m");
    m_baseUnitDict[SYSTEM_MKS]["cm"] = std::pair<double, std::string>(1e-2, "m");
    m_baseUnitDict[SYSTEM_MKS]["m"] = std::pair<double, std::string>(1, "m");
    m_baseUnitDict[SYSTEM_MKS][""] = std::pair<double, std::string>(1, "1");

    return ;
}       /* -----  end of method PhysicalUnit::InitPhysicalUnitDict  ----- */


std::string PhysicalUnit::GetName () const
{
    std::stringstream ss;
    std::map<std::string, int>::const_iterator it;
    bool isFirst = true;
    for (it = m_unitDict.begin(); it != m_unitDict.end(); ++it) {
        if (it->second == 0) continue;

        if (!isFirst) ss << " * ";

        ss << it->first;
        if (it->second != 1) ss << "^" << it->second;

        if (isFirst) isFirst = false;
    }
    return ss.str();
}       /* -----  end of method PhysicalUnit::GetName  ----- */

}       /* -----  end of namespace phy  ----- */
