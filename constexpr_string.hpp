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

#ifndef CONSTEXPR_STRING_HPP
#define	CONSTEXPR_STRING_HPP

#include "string.hpp"

namespace tml
{
    struct const_string
    {
        const char* string;
        const std::size_t length;
        
        template<typename INDEX>
        struct kernel
        {
            using result = tml::character<string[INDEX::value]>;
        };
        
        using string_type = tml::for_each<tml::make_size_t_forward_iterator<0> ,
                                          tml::make_size_t_forward_iterator<length>,
                                          kernel
                                         >;
        
        template<std::size_t N>
        constexpr const_string( const char (&str)[N] ) : 
            string{ str } ,
            length{ N }
        {}
    };
}

#endif	/* CONSTEXPR_STRING_HPP */

