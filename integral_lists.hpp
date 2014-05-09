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

#ifndef BASIC_LISTS_HPP
#define	BASIC_LISTS_HPP

#include "list.hpp"
#include "basic_types.hpp"

/*
 * This header defines aliases for lists of basic types, and lists of elements of an arbitrary type.
 */

namespace tml
{
    /*
     * Declares a list of values of type T.
     * 
     * T must be an integral type.
     * 
     * The result is a tml::list filled with the boxed equivalents of the input values.
     */
    template<typename T , T... Vs>
    using integral_list = tml::list<tml::integral_constant<T,Vs>...>;
    
    
    /*
     * Declares a list of characters
     */
    template<char... Vs>
    using character_list = tml::integral_list<char,Vs...>;
    
    /*
     * Declares a list of integers
     */
    template<int... Vs>
    using integer_list = tml::integral_list<int,Vs...>;
    
    /*
     * Declares a list of booleans
     */
    template<bool... Vs>
    using boolean_list = tml::integral_list<bool,Vs...>;
    
    /*
     * Declares a list of unsigned characters
     */
    template<unsigned char... Vs>
    using unsigned_character_list = tml::integral_list<unsigned char,Vs...>;
    
    /*
     * Declares a list of unsigned integers
     */
    template<unsigned int... Vs>
    using unsigned_integer_list = tml::integral_list<unsigned int,Vs...>;
}

#endif	/* BASIC_LISTS_HPP */

