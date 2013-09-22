/* 
 * File:   color.hpp
 * Author: manu343726
 *
 * Created on 19 de septiembre de 2013, 3:21
 */

#ifndef COLOR_HPP
#define	COLOR_HPP

#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "to_string.hpp"

#include <sstream>
#include <string>

namespace gfx
{
    template<mpl::character_type VALUE>
    using byte = mpl::ucharacter<VALUE>;
    
    template<typename R , typename G , typename B , typename A = gfx::byte<255>>
    struct color
    {
        using a = A;
        using r = R;
        using g = G;
        using b = B;
        
        constexpr operator unsigned int()
        {
            return ((unsigned int)A::value << 24) | ((unsigned int)R::value << 16) | ((unsigned int)G::value << 8) | (unsigned int)B::value;
        }
    };
}

namespace mpl
{
    template<typename A , typename R , typename G , typename B>
    struct to_string_t<gfx::color<R,G,B,A>>
    {
        operator std::string()
        {
            std::ostringstream os;
            
            os << "ARGB(" << (unsigned int)A::value << "," << (unsigned int)R::value << "," << (unsigned int)G::value << "," << (unsigned int)B::value << ")";
            
            return os.str();
        }
    };
}

#endif	/* COLOR_HPP */

