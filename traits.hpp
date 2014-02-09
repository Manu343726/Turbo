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

#ifndef TRAITS_HPP
#define	TRAITS_HPP

#include "basic_types.hpp"

#include <type_traits> //std::common_type

namespace
{
#define MAKE_TRAIT(type_name) template<typename T>                          \
                         struct is_##type_name : public tml::false_type {}; \
                                                                            \
                         template<tml::type_name##_type VALUE>              \
                         struct is_##type_name<tml::type_name<VALUE>> : public tml::true_type {}
}

namespace tml
{   
    template<typename T>
    struct is_value_t : public tml::false_type {};
    
    template<typename T , T VALUE>
    struct is_value_t<tml::value_t<T,VALUE>> : public tml::true_type {};
    
    
    template<typename T>
    struct is_type_t : public tml::false_type {};
    
    template<typename T>
    struct is_type_t<tml::type_t<T>> : public tml::true_type {};
    
    /*
    MAKE_TRAIT( ubyte );
    MAKE_TRAIT( byte );
    MAKE_TRAIT( uinteger );
    MAKE_TRAIT( integer );
    MAKE_TRAIT( ulong_integer );
    MAKE_TRAIT( long_integer );
    MAKE_TRAIT( ulong_long_integer );
    MAKE_TRAIT( long_long_integer );
    MAKE_TRAIT( boolean );
    MAKE_TRAIT( size_t );
    */
}

#endif	/* TRAITS_HPP */

