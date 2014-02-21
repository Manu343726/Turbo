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

#ifndef STRING_HPP
#define	STRING_HPP

#include "core/basic_types.hpp"
#include "list.hpp"
#include "for_loops.hpp"
#include "numeric_iterators.hpp"

#include <string>
#include <array>


namespace tml
{
#define TURBO_STRING_VARIABLE( name , string )                                                          \
    template<typename INDEX>                                                                            \
    using turbo_string_transform_function_##name = tml::function<tml::character<string[INDEX::value]>>; \
                                                                                                        \
    using name = tml::for_each<tml::make_size_t_forward_iterator<0>,tml::make_size_t_forward_iterator<sizeof(string)>,turbo_string_transform_function_##name>


    TURBO_DEFINE_FUNCTION( compact_string  , (typename STRING) , (STRING) );
    
    template<typename... CHARS , typename... TAIL>
    struct compact_string_t<tml::list<CHARS...,tml::character<0>,TAIL...>> : public tml::function<tml::list<CHARS...,tml::character<0>>> {};
    
    
    template<std::size_t N>
    using integer_sequence = tml::for_each<tml::make_size_t_forward_iterator<0> , tml::make_size_t_forward_iterator<N> , tml::function >;
    
    
    
    template<typename... INDICES , std::size_t N>
    constexpr auto compute_string( tml::list<INDICES...> , std::array<char,N> string )
    {
        return tml::list<tml::character<string[INDICES::value]>...>{}; 
    }
    
    template<std::size_t N>
    constexpr auto compute_string( const char (&string)[N] ) 
    {
        return compute_string( integer_sequence<N>{} , string );    
    }
    
#define TURBO_STRING( string ) decltype( tml::compute_string( string ) )
    
    
    template<tml::character_type... CHARS>
    struct to_string_t<tml::list<tml::character<CHARS>...>>
    {
        operator std::string() const
        {
            return std::string{ { CHARS... } };
        }
    };
    
    
    TURBO_DEFINE_FUNCTION( get_raw_string , (typename T) , (T) );
    
    template<tml::character_type... CHARS>
    struct get_raw_string_t<tml::list<tml::character<CHARS>...>>
    {
        struct array_holder
        {
            static const tml::character_type string[sizeof...(CHARS)] = { CHARS... };
        };
        
        using result = array_holder;
    };
}

#endif	/* STRING_HPP */

