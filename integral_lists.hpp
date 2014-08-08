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
#include "integral.hpp"
#include "integral_iterators.hpp"
#include "high_order_functions.hpp"
#include "lazy.hpp"
#include "to_runtime.hpp"

#include <array>

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
    
    /*
     * Declares a list of std::size_ts
     */
    template<std::size_t... Vs>
    using size_t_list = tml::integral_list<std::size_t,Vs...>;
    
    
    /*
     * Makes a range of an arbitrary integral type T on the interval [begin,end)
     */
    template<typename T , T begin , T end>
    using integral_range = tml::transform<tml::lazy<tml::function>,
                                          tml::integral_forward_iterators::make<tml::integral_constant<T,begin>>,
                                          tml::integral_forward_iterators::make<tml::integral_constant<T,end>>
                                         >;
    
    /*
     * Makes a range of integers on the interval [begin,end)
     */
    template<std::size_t begin , std::size_t end>
    using size_t_range = tml::integral_range<std::size_t,begin,end>;
    
    /*
     * Makes a range of chars on the interval [begin,end)
     */
    template<char begin , char end>
    using character_range = tml::integral_range<char,begin,end>;
    
    /*
     * Makes a range of unsigned chars on the interval [begin,end)
     */
    template<unsigned char begin , unsigned char end>
    using unsigned_character_range = tml::integral_range<unsigned char,begin,end>;
    
    /*
     * Makes a range of integers on the interval [begin,end)
     */
    template<int begin , int end>
    using integer_range = tml::integral_range<int,begin,end>;
    
    /*
     * Makes a range of long integers on the interval [begin,end)
     */
    template<long int begin , long int end>
    using long_integer_range = tml::integral_range<long int,begin,end>;
    
    /*
     * Makes a range of long long integers on the interval [begin,end)
     */
    template<long long int begin , long long int end>
    using long_long_integer_range = tml::integral_range<long long int,begin,end>;
    
    /*
     * Makes a range of unsigned integers on the interval [begin,end)
     */
    template<unsigned int begin , unsigned int end>
    using unsigned_integer_range = tml::integral_range<unsigned int,begin,end>;
    
    /*
     * Makes a range of unsigned long integers on the interval [begin,end)
     */
    template<unsigned long int begin , unsigned long int end>
    using unsigned_long_integer_range = tml::integral_range<unsigned long int,begin,end>;
    
    /*
     * Makes a range of unsigned long long integers on the interval [begin,end)
     */
    template<unsigned long long int begin , unsigned long long int end>
    using unsigned_long_long_integer_range = tml::integral_range<unsigned long long int,begin,end>;
    
    /*
     * to_runtime() specialization for homogeneous basic type lists
     */
    namespace impl
    {
        template<typename T , T... Vs>
        struct runtime_representation<tml::integral_list<T,Vs...>> : public tml::function<std::array<T,sizeof...(Vs)>>
        {};
        
        template<typename T , T... Vs>
        struct to_runtime<tml::integral_list<T,Vs...>>
        {
            static const std::array<T,sizeof...(Vs)>& execute()
            {
                static const std::array<T,sizeof...(Vs)> array = { Vs... };
                
                return array;
            }
        };
    }
}

#endif	/* BASIC_LISTS_HPP */

