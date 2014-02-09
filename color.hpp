/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013 - 2014, Manuel Sánchez Pérez                             *
*                                                                             *
* This file is part of The Turbo Library.                                     *
*                                                                             *
* The Turbo Library is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU Lesser General Public License as published by *
* the Free Software Foundation, version 2 of the License.                     *
*                                                                             *
* The Turbo Library is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU Lesser General Public License for more details.                         *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with The Turbo Library. If not, see <http://www.gnu.org/licenses/>.   *
******************************************************************************/

#ifndef COLOR_HPP
#define	COLOR_HPP

#include "core.hpp"
#include "to_string.hpp"

#include <sstream>
#include <string>

namespace gfx
{
    template<char VALUE>
    using byte = tml::value_t<char,VALUE>;
    
    using byte_t = unsigned char;
    
    template<typename R , typename G , typename B , typename A = gfx::byte<255>>
    struct color : public tml::uinteger<((unsigned int)A::value << 24) | ((unsigned int)R::value << 16) | ((unsigned int)G::value << 8) | (unsigned int)B::value>
    {
        using a = A;
        using r = R;
        using g = G;
        using b = B;
    };
    
    template<byte_t R , byte_t G , byte_t B>
    using from_rgb = gfx::color<gfx::byte<R>,gfx::byte<G>,gfx::byte<B>>;
    
    template<byte_t A , byte_t R , byte_t G , byte_t B>
    using from_argb = gfx::color<gfx::byte<R>,gfx::byte<G>,gfx::byte<B>,gfx::byte<A>>;
}

namespace tml
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

