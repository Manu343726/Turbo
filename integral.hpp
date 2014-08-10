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

#include <type_traits>
#include <string>
#include <sstream>

#include "algebra.hpp"
#include "function.hpp"
#include "to_string.hpp"
#include "to_runtime.hpp"

/*
 * This header defines the basic integral types of the library.
 * 
 * This library works by convention with template type parameters only, so values of basic types (Such as ints, 
 * booleans, chars, etc) are specified through boxing.
 * The implementation is free to use value parameters in certain situations, but the user have never use parameters
 * which are not types (Except in some construction aliases).
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
 * 
 * Also this header includes "algebra.hpp" and implements algebraic and logical operations for basic types.
 */

namespace tml
{
    /*
     * Turbo integral constant is defined as an alias of std::integral_constant to provide STL compatibility
     */
    template<typename T , T V>
    using integral_constant = std::integral_constant<T,V>;
    
    /*
     * Opposite metafunction implementation for integral types
     */
    template<typename T , T V>
    struct opposite<tml::integral_constant<T,V>> : public tml::function<tml::integral_constant<T,-V>>
    {};

    /*
     * Abs metafunction implementation for integral types
     */
    template<typename T , T V>
    struct abs<tml::integral_constant<T,V>> : public tml::function<tml::integral_constant<T,((V < 0) ? -V : V)>>
    {};
    
    /*
     * Log10 of an integral value
     * Returns only the integral part of the logarithm, but the algorithm is fast and has O(1) complexity.
     * Calling log10 with a negative value results in undefined behaviour.
     */
    template<typename T , T V>
    struct log10<tml::integral_constant<T,V>>
    {
        static constexpr const auto value = (std::uint64_t)V; //Promote to the wider known unsigned type.
        
        static constexpr const T log = (0x0000000000000000ULL <= value && value < 0x000000000000000AULL) ? 0 :  //Interval [10^0,10^1)
                                       (0x000000000000000AULL <= value && value < 0x0000000000000064ULL) ? 1 :  //Interval [10^1,10^2)
                                       (0x0000000000000064ULL <= value && value < 0x00000000000003E8ULL) ? 2 :  //Interval [10^2,10^3)
                                       (0x00000000000003E8ULL <= value && value < 0x0000000000002710ULL) ? 3 :  //Interval [10^3,10^4)
                                       (0x0000000000002710ULL <= value && value < 0x00000000000186A0ULL) ? 4 :  //Interval [10^4,10^5)
                                       (0x00000000000186A0ULL <= value && value < 0x00000000000F4240ULL) ? 5 :  //Interval [10^5,10^6)
                                       (0x00000000000F4240ULL <= value && value < 0x0000000000989680ULL) ? 6 :  //Interval [10^6,10^7)
                                       (0x0000000000989680ULL <= value && value < 0x0000000005F5E100ULL) ? 7 :  //Interval [10^7,10^8)
                                       (0x0000000005F5E100ULL <= value && value < 0x000000003B9ACA00ULL) ? 8 :  //Interval [10^8,10^9)
                                       (0x000000003B9ACA00ULL <= value && value < 0x00000002540BE400ULL) ? 9 :  //Interval [10^9,10^10)
                                       (0x00000002540BE400ULL <= value && value < 0x000000174876E800ULL) ? 10 : //Interval [10^10,10^11)
                                       (0x000000174876E800ULL <= value && value < 0x000000E8D4A51000ULL) ? 11 : //Interval [10^11,10^12)
                                       (0x000000E8D4A51000ULL <= value && value < 0x000009184E72A000ULL) ? 12 : //Interval [10^12,10^13)
                                       (0x000009184E72A000ULL <= value && value < 0x00005AF3107A4000ULL) ? 13 : //Interval [10^13,10^14)
                                       (0x00005AF3107A4000ULL <= value && value < 0x00038D7EA4C68000ULL) ? 14 : //Interval [10^14,10^15)
                                       (0x00038D7EA4C68000ULL <= value && value < 0x002386F26FC10000ULL) ? 15 : //Interval [10^15,10^16)
                                       (0x002386F26FC10000ULL <= value && value < 0x016345785D8A0000ULL) ? 16 : //Interval [10^16,10^17)
                                       (0x016345785D8A0000ULL <= value && value < 0x0DE0B6B3A7640000ULL) ? 17 : //Interval [10^17,10^18)
                                                                                                           18;  //Interval [10^18,2^64 - 1)
        
        using result = tml::integral_constant<T,log>;
    };

    namespace impl
    {
        /*
         * Algebra primitives zero and one for integral values
         */
        template<typename T , T V>
        struct zero<tml::integral_constant<T,V>> : public tml::function<tml::integral_constant<T,0>>
        {};
        
        template<typename T , T V>
        struct one<tml::integral_constant<T,V>> : public tml::function<tml::integral_constant<T,1>>
        {};
        
        /*
         * Sign metafunction implementation for integral types
         */
        template<typename T , T V>
        struct sign<tml::integral_constant<T,V>> : public tml::function<tml::integral_constant<bool,(V >= 0)>>
        {};
        
        
        /*
         * to_string override for basic types:
         */
        template<typename T , T V>
        struct to_string<tml::integral_constant<T,V>>
        {
            operator std::string() const
            {
                return std::to_string( V );
            }
        };
        
        /*
         * Specialization for booleans
         */
        template<bool V>
        struct to_string<tml::integral_constant<bool,V>>
        {
            operator std::string() const
            {
                std::stringstream ss;
                
                ss << std::boolalpha << V;
                
                return ss.str();
            }
        };
        
        /*
         * Specialization for chars
         */
        template<char V>
        struct to_string<tml::integral_constant<char,V>>
        {
            operator std::string() const
            {
                std::stringstream ss;
                
                ss << V;
                
                return ss.str();
            }
        };
        
        /*
         * to_runtime() override for basic types
         */
        
        template<typename T , T V>
        struct runtime_representation<tml::integral_constant<T,V>> : public tml::function<T>
        {};
        
        template<typename T , T V>
        struct to_runtime<tml::integral_constant<T,V>>
        {
            static constexpr T execute()
            {
                return V;
            }
        };
    }

    /*
     * Boolean constants defined exactly as in the satndard library
     */
    using true_type = std::true_type;
    using false_type = std::false_type;
    
    /*
     * Turbo std::size_t boxing wrapper
     */
    template<std::size_t V>
    using size_t = tml::integral_constant<std::size_t,V>;
    

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


/* Algebra for basic types implementation */

namespace tml
{
    template<typename TLHS , TLHS VLHS , 
             typename TRHS , TRHS VRHS>
    struct add<tml::integral_constant<TLHS,VLHS> , tml::integral_constant<TRHS,VRHS>> :
        public tml::function<tml::integral_constant<decltype(VLHS + VRHS),VLHS + VRHS>>
    {};
    
    template<typename TLHS , TLHS VLHS , 
             typename TRHS , TRHS VRHS>
    struct sub<tml::integral_constant<TLHS,VLHS> , tml::integral_constant<TRHS,VRHS>> :
        public tml::function<tml::integral_constant<decltype(VLHS - VRHS),VLHS - VRHS>>
    {};
    
    template<typename TLHS , TLHS VLHS , 
             typename TRHS , TRHS VRHS>
    struct mul<tml::integral_constant<TLHS,VLHS> , tml::integral_constant<TRHS,VRHS>> :
        public tml::function<tml::integral_constant<decltype(VLHS * VRHS),VLHS * VRHS>>
    {};
    
    template<typename TLHS , TLHS VLHS , 
             typename TRHS , TRHS VRHS>
    struct div<tml::integral_constant<TLHS,VLHS> , tml::integral_constant<TRHS,VRHS>> :
        public tml::function<tml::integral_constant<decltype(VLHS / VRHS),VLHS / VRHS>>
    {};
    
    
    template<bool VLHS , 
             bool VRHS>
    struct logical_or<tml::boolean<VLHS> , tml::boolean<VRHS>> :
        public tml::function<tml::boolean<VLHS || VRHS>>
    {};
    
    template<bool VLHS , 
             bool VRHS>
    struct logical_and<tml::boolean<VLHS> , tml::boolean<VRHS>> :
        public tml::function<tml::boolean<VLHS && VRHS>>
    {};
    
    template<bool OP>
    struct logical_not<tml::boolean<OP>> :
        public tml::function<tml::boolean<!OP>>
    {};
    
    template<typename TLHS , TLHS VLHS , 
             typename TRHS , TRHS VRHS>
    struct equal<tml::integral_constant<TLHS,VLHS> , tml::integral_constant<TRHS,VRHS>> :
        public tml::function<tml::boolean<VLHS == VRHS>>
    {};
    
    template<typename TLHS , TLHS VLHS , 
             typename TRHS , TRHS VRHS>
    struct less_than<tml::integral_constant<TLHS,VLHS> , tml::integral_constant<TRHS,VRHS>> :
        public tml::function<tml::boolean<(VLHS < VRHS)>>
    {};
}

#endif	/* BASIC_TYPES_HPP */

