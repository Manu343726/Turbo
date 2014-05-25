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

#ifndef FUNCTION_HPP
#define	FUNCTION_HPP

#include <type_traits>

namespace tml
{
    namespace impl
    {
        /*
         * The library assumes that any type with a 'result' member type is a function
         * (A function which result is stored in that 'result' member).
         * 
         * This is an internal (not dessigned to be used by the user) type trait to 
         * check the existence of that member type, that is, to check if a type is
         * a function.
         */
        template<typename T>
        struct is_function
        {
            template<typename U> static std::true_type test( typename U::result* );
            template<typename U> static std::false_type test( ... );
            
            static constexpr bool result = decltype( test<T>( nullptr ) )::value;
        };
    }
    
    /* User-side tml::is_function type-trait 
     * 
     * Checks if a given expression is a function according to the conventions of the library
     * (Has a 'result' member type)
     */
    template<typename E>
    using is_function = std::integral_constant<bool,tml::impl::is_function<E>::result>;
    
    /*
     * This is a helper metafunction to represent a metafunction in the way the library
     * expects it.
     * 
     * It just takes a value and stores it in a 'result' member.
     * 
     * Its usefull when declaring user-defined metafunctions and making them working with the rest
     * of the library. Inheriting from this helper is a simple whay to ensure any metafunction
     * has the correct interface.
     */
    template<typename RESULT>
    struct function
    {
        using result = RESULT;
    };
}

#endif	/* FUNCTION_HPP */

