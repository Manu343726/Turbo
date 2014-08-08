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

#ifndef UTILITY_HPP
#define	UTILITY_HPP

#include "integral.hpp"
#include "function.hpp"

#include <climits>

/*
 * The namespace tml::util contains a set sparse metaprogramming utilities.
 */

namespace tml
{
    namespace util
    {
        namespace func
        {
            template<typename... Ts>
            struct pack_length : public tml::function<tml::size_t<sizeof...(Ts)>>
            {};
            
            template<typename T>
            struct sizeof_bits : public tml::function<tml::size_t<sizeof(T) * CHAR_BIT>>
            {};
            
            template<typename T>
            struct size_of : public tml::function<tml::size_t<sizeof(T)>>
            {};
        }
        
        /*
         * Computes the length of a variadic pack
         */
        template<typename... ARGS>
        using pack_length = typename tml::util::func::pack_length<ARGS...>::result;
        
        /*
         * Computes the size in bits of a certain type T
         */
        template<typename T>
        using sizeof_bits = typename tml::util::func::sizeof_bits<T>::result;
        
        /*
         * Computes the size in bytes of a certain type T
         */
        template<typename T>
        using size_of = typename tml::util::func::size_of<T>::result;
    }
}

#endif	/* UTILITY_HPP */

