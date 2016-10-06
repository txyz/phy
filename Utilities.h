/*
 * =====================================================================================
 *
 *       Filename:  Utilities.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2016 06:29:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongwei Yang (yhw1630), yhw1630@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  PHY_Utilities_INC
#define  PHY_Utilities_INC

#include <string>
#include <vector>

namespace phy {
namespace utl {

std::vector<std::string> split(const std::string &s, char delim);
std::string& ltrim(std::string &s, char c=0);
std::string &rtrim(std::string &s, char c=0);
std::string& trim(std::string &s, char c=0);

}       /* -----  end of namespace utl  ----- */
}       /* -----  end of namespace phy  ----- */
#endif   /* ----- #ifndef Utilities_INC  ----- */
