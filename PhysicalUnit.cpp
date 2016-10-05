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
 *         Author:  Hongwei Yang (yhw1630), yhw1630@gmail.com
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

namespace phy {

std::map<std::string, std::map<std::string, std::pair<double, std::string> > > PhysicalUnit::m_baseUnitDict;

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

// trim from left
std::string& ltrim(std::string &s, char c=0) {
    if (c) {
        size_t npos = s.find_first_not_of(c);
        s.erase(0, npos);
    } else {
        s.erase(s.begin(),
                std::find_if(s.begin(),
                             s.end(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))));
    }
    return s;
}

// trim from right
std::string &rtrim(std::string &s, char c=0) {
    if (c) {
        size_t npos = s.find_last_not_of(c);
        s.erase(npos);
    } else {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                s.end());
    }
    return s;
}

// trim from both
std::string& trim(std::string &s, char c=0) {return ltrim(rtrim(s, c), c);}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  PhysicalUnit
 *      Method:  PhysicalUnit
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
PhysicalUnit::PhysicalUnit (const std::string &name, const std::string &system)
{
    InitBaseUnitDict();
    if (m_baseUnitDict.find(system) == m_baseUnitDict.end()) {
        std::stringstream ss;
        ss << "Unknown system of measure: " << system;
        throw std::invalid_argument(ss.str());
    }

    m_system = system;

    std::map<std::string, std::pair<double, std::string> > &unitDict = m_baseUnitDict[system];

    // Composite unit parser, currently support '*', '/', '^', do not support '(', ')'
    std::map<std::string, std::pair<double, std::string> >::iterator it;
    double scale;
    std::string unitName, baseName;
    // '*' first
    std::vector<std::string> splitted1 = split(name, '*');
    std::vector<std::string>::iterator it1;
    bool isDenominator;
    for (it1 = splitted1.begin(); it1 != splitted1.end(); ++it1) {
        isDenominator = false;
        // Now '/'
        trim(*it1);
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
        std::vector<std::string> splitted2 = split(*it1, '/');
        std::vector<std::string>::iterator it2;
        for (it2 = splitted2.begin(); it2 != splitted2.end(); ++it2) {
            trim(*it2);
            std::vector<std::string> splitted3 = split(*it2, '^');
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
                if (isDenominator) order = -order;
                unitName = splitted3[0];
                it = unitDict.find(unitName);
                if (it == unitDict.end()) {
                    std::stringstream ss;
                    ss << "Unknown unit: " << unitName;
                    throw std::invalid_argument(ss.str());
                }
                scale = it->second.first;
                baseName = it->second.second;
                if (m_nameDict.find(unitName) != m_nameDict.end()) {
                    m_nameDict[unitName] += order;
                } else {
                    m_nameDict[unitName] = order;
                }
            }
            // Only after first element, it becomes denominator
            isDenominator = true;
        }
    }

    //"um", "mm", "cm", "m", "km"}
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
    m_nameDict(other.m_nameDict)
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
        m_nameDict = other.m_nameDict;
    }
    return *this;
}  /* -----  end of method PhysicalUnit::operator =  (assignment operator)  ----- */

void PhysicalUnit::InitBaseUnitDict ()
{
    if (!m_baseUnitDict.empty()) return;

    m_baseUnitDict["SI"]["nm"] = std::pair<double, std::string>(1e-9, "m");
    m_baseUnitDict["SI"]["um"] = std::pair<double, std::string>(1e-6, "m");
    m_baseUnitDict["SI"]["mm"] = std::pair<double, std::string>(1e-3, "m");
    m_baseUnitDict["SI"]["cm"] = std::pair<double, std::string>(1e-2, "m");
    m_baseUnitDict["SI"]["m"] = std::pair<double, std::string>(1, "m");
    m_baseUnitDict["SI"][""] = std::pair<double, std::string>(1, "1");

    return ;
}       /* -----  end of method PhysicalUnit::InitBaseUnitDict  ----- */

std::string PhysicalUnit::GetName () const
{
    std::stringstream ss;
    std::map<std::string, int>::const_iterator it;
    for (it = m_nameDict.begin(); it != m_nameDict.end(); ++it) {
        if (it != m_nameDict.begin()) {
            ss << " * ";
        }
        ss << it->first;
        if (it->second != 1) ss << "^" << it->second;
    }
    return ss.str();
}       /* -----  end of method PhysicalUnit::GetName  ----- */

std::string PhysicalUnit::GetBaseName () const
{
    std::stringstream ss;
    std::map<std::string, int>::const_iterator it;
    for (it = m_nameDict.begin(); it != m_nameDict.end(); ++it) {
        if (it != m_nameDict.begin()) {
            ss << " * ";
        }
        ss << m_baseUnitDict[m_system].find(it->first)->second.second;
        if (it->second != 1) ss << "^" << it->second;
    }
    return ss.str();
}       /* -----  end of method PhysicalUnit::GetBaseName  ----- */

double PhysicalUnit::GetScale () const
{
    double scale = 1;
    std::map<std::string, int>::const_iterator it;
    for (it = m_nameDict.begin(); it != m_nameDict.end(); ++it) {
        scale *= pow(m_baseUnitDict[m_system].find(it->first)->second.first, it->second);
    }
    return scale;
}       /* -----  end of method PhysicalUnit::GetScale  ----- */

}       /* -----  end of namespace phy  ----- */
