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

#ifndef FIXED_POINT_HPP
#define	FIXED_POINT_HPP

#include <cstdint>
#include <string>

#include "algebra.hpp"
#include "function.hpp"
#include "utility.hpp"
#include "basic_types.hpp"
#include "algorithm.hpp"

/*
 * This header defines the template tml::fixed_point, which implements arbitrary-length fixed-point 
 * arithmetic.
 */

namespace tml
{
    namespace impl
    {
        /*
         * Turbo uses the first quarter (From the LSB side) of the underlying integer number for decimals.
         * That is, for tml::fsingle, which uses 32 bit integers, the first 8 bits are for decimals and the rest
         * for the integer part:
         * 
         *                 32 bits
         *  <----------------------------------->
         *  +------------------------+----------+
         *  |         Integer        | Decimals |
         *  +------------------------+----------+
         *  <-----------------------><---------->
         *            24 bits           8 bits
         * 
         * The templates 'tml::impl::decimal_bits' and 'tml::impl::integer_bits' compute the size on bits of a
         * fixed-point part given an underlying integer type.
         * 
         * NOTE: Here I miss C++14 variable templates, but I want to make the library compatible with C++11 at least.
         */
        template<typename INTEGER_T>
        using integer_bits = tml::size_t<(tml::util::sizeof_bits<INTEGER_T>::value * 3) / 4>;
        
        template<typename INTEGER_T>
        using decimal_bits = tml::size_t<tml::util::sizeof_bits<INTEGER_T>::value / 4>;
        
        /*
         * The following templates return the position (Index) of the most significant bit (MSB) and less significant bit
         * (MSB) of the integer and decimal parts of a fixed-point number respectively, given the underlying integer type used.
         */
        template<typename INTEGER_T>
        using integer_lsb = tml::impl::decimal_bits<INTEGER_T>;
        
        template<typename INTEGER_T>
        using integer_msb = tml::size_t<tml::util::sizeof_bits<INTEGER_T>::value - 1>;
        
        template<typename INTEGER_T>
        using decimal_lsb = tml::size_t<0>;
        
        template<typename INTEGER_T>
        using decimal_msb = tml::size_t<tml::impl::decimal_bits<INTEGER_T>::value - 1>;
        
        /*
         * Underlying integer type of tml::fsingle single precision fixed-point type 
         */
        using single_t = std::int32_t;
        
        /*
         * Underlying integer type of tml::fdouble double precision fixed-point type 
         */
        using double_t = std::int64_t;
    }
    
    /*
     * Fixed-point type of an arbitrary underlying integer type.
     */
    template<typename INTEGER_T , INTEGER_T N>
    struct fixed_point
    {};
    
    /*
     * Aliases for library-defined single and double precision fixed-point types.
     * 
     * The implementation of fixed-point types is dessigned to work with any underlying integer type, but
     * the library defines two aliases: One for single precision (32 bit, 24 integer bits and 8 decimal bits) and
     * one for double precision (64 bit, 48 integer bits and 16 decimal bits).
     * 
     * Note that type conversions are not implemented, so fixed-point arithmetic is only valid between fixed-point types with
     * the same underlying integer type. In other words, is not possible to do algebra between tml::fsingle and tml::fdouble.
     * Depending on the requeriments, the user should choose one fixed-point type.
     */
    template<tml::impl::single_t N>
    using fsingle = tml::fixed_point<tml::impl::single_t,N>;
    
    template<tml::impl::double_t N>
    using fdouble = tml::fixed_point<tml::impl::double_t,N>;
    
    
    /*
     * Arithmetic and boolean operations for fixed-point types
     */

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct add<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
        public tml::function<tml::fixed_point<INTEGER_T,LHS + RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct sub<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
        public tml::function<tml::fixed_point<INTEGER_T,LHS - RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct mul<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
        public tml::function<tml::fixed_point<INTEGER_T,((LHS * RHS) >> tml::impl::decimal_bits<INTEGER_T>::value)>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct div<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
    public tml::function<tml::fixed_point<INTEGER_T,(LHS << tml::impl::decimal_bits<INTEGER_T>::value ) / RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct equal<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
        public tml::function<tml::boolean<LHS == RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct less_than<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
        public tml::function<tml::boolean<(LHS < RHS)>>
    {};
        
    namespace impl
    {
        /*
         * tml::to_string override
         */
        template<typename INTEGER_T , INTEGER_T N>
        struct to_string<tml::fixed_point<INTEGER_T,N>>
        {
            operator std::string() const
            {
                return std::to_string( N / static_cast<double>( 1 << tml::impl::decimal_bits<INTEGER_T>::value ) );
            }
        };
    }
}

#endif	/* FIXED_POINT_HPP */

