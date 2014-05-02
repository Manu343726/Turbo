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


/*
 * This header defines the basic types of the library.
 * 
 * This library works by convention with template type parameters only, so values of basic types (Such as ints, 
 * booleans, chars, etc) are specified through boxing.
 * The implementation is free to use value parameters in certain situations, but the user have never use parameters
 * which are not types.
 * 
 * NOTE: Template parameters are disallowed too. For that purpose, the library provides wrappers to pass templates through
 * boxing (See tml::lazy in "lazy.hpp").
 * 
 * 
 * This header defines one boxing template for each integral type of the C++ programming language. All templates are defined as
 * aliases of the template 'tml::integral_constant', which is an alias of 'std::integral_constant (To provide compatibility with the 
 * C++ Standard Library).
 * The library defines basic types using the following naming conventions:
 * 
 *  - Same name of the C++ type, using CammelCase: This case allows us to use the same common name without having conflicts with keywords
 *    of the language. This is the default naming convention. 
 *    Some examples:
 * 
 *        using a = tml::Int<0>;            // int a = 0
 *        using b = tml::Bool<true>;        // int b = true
 *        using c = tml::UnsignedChar<'a'>; // int c = 'a'
 *        
 *  - Verbose names, lower case, and underscores: This naming convention tries to mimic the naming convention of the Standard Library, convention
 *    followed by the Turbo Metaprogramming Library. Even if using this naming convention is uniform respect the other features of the library,
 *    such names could be a bit verbose and long (Of course, thats really a subjective opinion. Thats why the library defines multiple naming conventions,
 *    to let the user decide what convention is more readable).
 * 
 *        using a = tml::integer<0>;              // int a = 0
 *        using b = tml::boolean<true>;           // int b = true
 *        using c = tml::unsigned_character<'a'>; // int c = 'a'
 * 
 *    Also prefix-based names for unsigned  and long types are provided:
 * 
 *        using d = tml::ulinteger<2>; // unsigned long int d = 0
 * 
 *  - C++ keywords followed with an underscore: Thats the naming convention followed by the Boost::mpl metaprogramming library:
 * 
 *        using a = tml::int_<0>;     // int a = 0
 *        using b = tml::bool_<true>; // int b = true
 *        using c = tml::char_<'a'>;  // int c = 'a'
 * 
 *    WARNING: CURRENTLY NOT IMPLEMENTED
 */

namespace tml
{
    /*
     * Turbo integral constant is defined as an alias of std::integral_constant to provide STL compatibility
     */
    template<typename T , T V>
    using integral_constant = std::integral_constant<T,V>;

    /*
     * Boolean constants defined exactly as in the satndard library
     */
    using true_type = std::true_type;
    using false_type = std::false_type;
    

    /*
     * Basic types are declared in this custom namespace "basic_types_declarations".
     * Later, the tml and global namespaces include that namespace with using directives.
     * The effect is to have the types declared both in global and tml namespace scopes without name collisions.
     */
    namespace basic_types_declarations
    {
        /* CamelCase naming convention 
         *****************************/

        /*
         * Boolean constants in camel case
         */
        using True = tml::true_type;
        using False = tml::false_type;

        template<char V>
        using Char = tml::integral_constant<char,V>;
        template<int V>
        using Int = tml::integral_constant<int,V>;
        template<bool V>
        using Bool = tml::integral_constant<bool,V>;

        template<unsigned char V>
        using UnsignedChar = tml::integral_constant<unsigned char,V>;
        template<unsigned int V>
        using UnsignedInt = tml::integral_constant<unsigned int,V>;

        template<long int V>
        using LongInt = tml::integral_constant<long int,V>;
        template<long long int V>
        using LongLongInt = tml::integral_constant<long long int,V>;

        template<unsigned long int V>
        using UnsignedLongInt = tml::integral_constant<unsigned long int,V>;
        template<unsigned long long int V>
        using UnsignedLongLongInt = tml::integral_constant<unsigned long long int,V>;



        /* CamelCase naming convention 
         *****************************/

        template<char V>
        using character = tml::integral_constant<char,V>;
        template<int V>
        using integer = tml::integral_constant<int,V>;
        template<bool V>
        using boolean = tml::integral_constant<bool,V>;

        template<unsigned char V>
        using unsigned_character = tml::integral_constant<unsigned char,V>;
        template<unsigned int V>
        using unsigned_integer = tml::integral_constant<unsigned int,V>;

        template<long int V>
        using long_integer = tml::integral_constant<long int,V>;
        template<long long int V>
        using long_long_integer = tml::integral_constant<long long int,V>;

        template<unsigned long int V>
        using unsigned_long_integer = tml::integral_constant<unsigned long int,V>;
        template<unsigned long long int V>
        using unsigned_long_long_integer = tml::integral_constant<unsigned long long int,V>;

        template<unsigned char V>
        using ucharacter = tml::integral_constant<unsigned char,V>;
        template<unsigned int V>
        using uinteger = tml::integral_constant<unsigned int,V>;

        template<long int V>
        using linteger = tml::integral_constant<long int,V>;
        template<long long int V>
        using llinteger = tml::integral_constant<long long int,V>;

        template<unsigned long int V>
        using ulinteger = tml::integral_constant<unsigned long int,V>;
        template<unsigned long long int V>
        using ullinteger = tml::integral_constant<unsigned long long int,V>;
    }
    
    using namespace tml::basic_types_declarations;
}

using namespace tml::basic_types_declarations;

#endif	/* BASIC_TYPES_HPP */

