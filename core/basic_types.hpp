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

#ifndef BASIC_TYPES_HPP
#define	BASIC_TYPES_HPP

#include "value_t.hpp"

namespace make_type_macro
{
    #define MAKE_TYPE(name , type) template<type VALUE>                   \
                                   using name = value_t<type,VALUE>; \
                                   using name##_type = type
}

namespace tb
{
    /* unmeaning sentinel type */
    
    struct no_type {};
    
    using nil = no_type;
    using null = nil;
    
    template<typename T>
    struct type_t
    {
        using type = T;
    };
    
    
    template<typename T , typename U>
    struct pair
    {
        using first  = T;
        using second = U;
    };
    
    MAKE_TYPE(ucharacter         , unsigned char);
    MAKE_TYPE(character          , char);
    MAKE_TYPE(uinteger           , unsigned int);
    MAKE_TYPE(integer            , int);
    MAKE_TYPE(ulong_integer      , unsigned long int);
    MAKE_TYPE(long_integer       , long int);
    MAKE_TYPE(ulong_long_integer , unsigned long long int);
    MAKE_TYPE(long_long_integer  , long long int);
    MAKE_TYPE(boolean            , bool);
    MAKE_TYPE(size_t             , std::size_t);
  
    
    using false_type = tb::boolean<false>;
    using true_type  = tb::boolean<true>;
}

#endif	/* BASIC_TRAITS_HPP */

