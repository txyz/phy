/*
 * =====================================================================================
 *
 *       Filename:  Streamable.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2016 10:59:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongwei Yang (yhw1630), yhw1630@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  PHY_Streamable_INC
#define  PHY_Streamable_INC

#include <string>
#include <sstream>

namespace phy {
/*
 * =====================================================================================
 *        Class:  Streamable
 *  Description:  
 * =====================================================================================
 */
class Streamable
{
  public:
    /* ====================  LIFECYCLE     ======================================= */
    Streamable () {}                            /* constructor      */
    Streamable (const Streamable &other) {}     /* copy constructor */
    virtual ~Streamable () {}                   /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    Streamable& operator = (const Streamable &other) {
        if (this != &other) {
        }
        return *this;
    } /* assignment operator */

    /* ====================  METHODS       ======================================= */
    virtual std::string ToString() const = 0;

  protected:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  METHODS       ======================================= */

    /* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class Streamable  ----- */

static inline std::ostream& operator << (std::ostream &os, const Streamable &obj)
{
    os << obj.ToString();
    return os;
}       /* -----  end of function operator <<  ----- */

}       /* -----  end of namespace phy  ----- */

#endif   /* ----- #ifndef Streamable_INC  ----- */
