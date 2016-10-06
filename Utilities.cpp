/*
 * =====================================================================================
 *
 *       Filename:  Utilities.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2016 06:30:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongwei Yang (yhw1630), yhw1630@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Utilities.h"

#include <sstream>

namespace phy {
namespace utl {

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
std::string& ltrim(std::string &s, char c) {
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
std::string &rtrim(std::string &s, char c) {
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
std::string& trim(std::string &s, char c) {return ltrim(rtrim(s, c), c);}

}       /* -----  end of namespace utl  ----- */
}       /* -----  end of namespace phy  ----- */
