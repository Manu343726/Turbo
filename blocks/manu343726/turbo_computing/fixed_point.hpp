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

#include "manu343726/turbo_core/algebra.hpp"
#include "manu343726/turbo_core/function.hpp"
#include "manu343726/turbo_utils/utility.hpp"
#include "manu343726/turbo_core/integral.hpp"
#include "manu343726/turbo_core/algorithm.hpp"
#include "manu343726/turbo_core/lambda.hpp"
#include "manu343726/turbo_core/placeholders.hpp"
#include "manu343726/turbo_core/runtime_placeholders.hpp"
#include "manu343726/turbo_core/to_runtime.hpp"

using namespace tml::placeholders;
using tml::runtime::placeholders::__;

/*
 * This header defines the template tml::fixed::number, which implements arbitrary-length fixed-point 
 * arithmetic.
 */

namespace tml
{
    namespace fixed
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
             * The templates 'tml::fixed::impl::decimal_bits' and 'tml::fixed::impl::integer_bits' compute the size on bits of a
             * fixed-point part given an underlying integer type.
             * 
             * This is the default config. Turbo uses the macro TURBO_DEBUG_FP_DECIMAL_PART to specify the fraction
             * of bits which are used for decimals, which of course is 4 by default.
             * 
             * NOTE: Here I miss C++14 variable templates, but I want to make the library compatible with C++11 at least.
             */

    #ifndef TURBO_DEBUG_FP_DECIMAL_PART
    #define TURBO_DEBUG_FP_DECIMAL_PART 4
    #endif /* TURBO_DEBUG_FP_DECIMAL_PART */

            template<typename INTEGER_T>
            using decimal_bits = tml::size_t<tml::util::sizeof_bits<INTEGER_T>::value / (TURBO_DEBUG_FP_DECIMAL_PART)>;

            template<typename INTEGER_T>
            using integer_bits = tml::size_t<(tml::util::sizeof_bits<INTEGER_T>::value - tml::fixed::impl::decimal_bits<INTEGER_T>::value)>;

            /*
             * The following templates return the position (Index) of the most significant bit (MSB) and less significant bit
             * (LSB) of the integer and decimal parts of a fixed-point number respectively, given the underlying integer type used.
             */
            template<typename INTEGER_T>
            using integer_lsb = tml::fixed::impl::decimal_bits<INTEGER_T>;

            template<typename INTEGER_T>
            using integer_msb = tml::size_t<tml::util::sizeof_bits<INTEGER_T>::value - 1>;

            template<typename INTEGER_T>
            using decimal_lsb = tml::size_t<0>;

            template<typename INTEGER_T>
            using decimal_msb = tml::size_t<tml::fixed::impl::decimal_bits<INTEGER_T>::value - 1>;

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
        struct number
        {};

        /*
         * This alias instances a fixed-point value adjusting automatically the number to the underlying
         * integer. That is, the value passed is not the value of the underlying integer, is an integral value
         * for the fixed-point number
         */
        template<typename INTEGER_T , INTEGER_T V>
        using normalize = tml::fixed::number<INTEGER_T , ( V << tml::fixed::impl::decimal_bits<INTEGER_T>::value )>;

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
        template<tml::fixed::impl::single_t N>
        using fsingle = tml::fixed::number<tml::fixed::impl::single_t,N>;

        template<tml::fixed::impl::double_t N>
        using fdouble = tml::fixed::number<tml::fixed::impl::double_t,N>;



        
        
        namespace impl
        {
            /*
             * Casting metafunction from integral to fixed-point values
             */
            template<typename T , typename FIXED>
            struct to_integral;

            /*
             * Casting metafunction from fixed-point to integral values
             */
            template<typename INTEGER_T , typename INTEGRAL>
            struct to_fixed;

            /*
             * integral -> fixed-point cast.
             */
            template<typename INTEGER_T, typename T , T V>
            struct to_fixed<INTEGER_T,tml::integral_constant<T,V>> : public tml::function<tml::fixed::number<INTEGER_T,(V << tml::fixed::impl::decimal_bits<INTEGER_T>::value)>>
            {};

            /*
             * fixed-point -> integral cast.
             */
            template<typename T , typename INTEGER_T , INTEGER_T V>
            struct to_integral<T,tml::fixed::number<INTEGER_T,V>> : public tml::function<tml::integral_constant<T,(V >> tml::fixed::impl::decimal_bits<INTEGER_T>::value)>>
            {};

            /*
             * integral -> fixed-point cast (Using value directly instead of type tag).
             */
            template<typename INTEGER_T, INTEGER_T W , typename T , T V>
            struct to_fixed<tml::fixed::number<INTEGER_T,W>,tml::integral_constant<T,V>> : public tml::fixed::impl::to_fixed<INTEGER_T,tml::integral_constant<T,V>>
            {};

            /*
             * fixed-point -> integral cast (Using value directly instead of type tag).
             */
            template<typename T , T W , typename INTEGER_T , INTEGER_T V>
            struct to_integral<tml::integral_constant<T,W>,tml::fixed::number<INTEGER_T,V>> : public tml::fixed::impl::to_integral<T,tml::fixed::number<INTEGER_T,V>>
            {};
        }

        namespace func
        {
            /*
             * Casting metafunction from fixed-point to integral values
             */
            template<typename T , typename FIXED>
            using to_integral = tml::fixed::impl::to_integral<T,FIXED>;

            /*
             * Casting metafunction from integral to fixed-point values
             */
            template<typename INTEGER_T , typename INTEGRAL>
            using to_fixed = tml::fixed::impl::to_fixed<INTEGER_T,INTEGRAL>;
        }

        /*
         * Casting metafunction from fixed-point to integral values
         */
        template<typename T , typename FIXED>
        using to_integral = typename tml::func::fixed::to_integral<T,FIXED>::result;

        /*
         * Casting metafunction from integral to fixed-point values
         */
        template<typename INTEGER_T , typename INTEGRAL>
        using to_fixed = typename tml::func::fixed::to_fixed<INTEGER_T,INTEGRAL>::result;

        /*
         * Casting metafunction from integral to single-precision fixed-point values
         */
        template<typename INTEGRAL>
        using to_fsingle = tml::fixed::to_fixed<tml::fixed::impl::single_t,INTEGRAL>;

        /*
         * Casting metafunction from integral to double-precision fixed-point values
         */
        template<typename INTEGRAL>
        using to_fdouble = typename tml::fixed::to_fixed<tml::fixed::impl::double_t,INTEGRAL>;

        /*
         * Alias for single-precision fixed-point zero value
         */
        using fszero = tml::zero<tml::fixed::fsingle<__>>;

        /*
         * Alias for single-precision fixed-point one value
         */
        using fsone = tml::one<tml::fixed::fsingle<__>>;

        /*
         * Alias for double-precision fixed-point zero value
         */
        using fdzero = tml::zero<tml::fixed::fdouble<__>>;

        /*
         * Alias for double-precision fixed-point one value
         */
        using fdone = tml::one<tml::fixed::fdouble<__>>;
    }

    /*
     * Opposite metafunction implementation for fixed-point types
     */
    template<typename INTEGER_T , INTEGER_T V>
    struct opposite<tml::fixed::number<INTEGER_T,V>> : public tml::function<tml::fixed::number<INTEGER_T,-V>>
    {};

    /*
     * Abs metafunction implementation for fixed-point types
     */
    template<typename INTEGER_T , INTEGER_T V>
    struct abs<tml::fixed::number<INTEGER_T,V>> : public tml::function<tml::fixed::number<INTEGER_T,((V > 0) ? -V : V)>>
    {};



    /*
     * Arithmetic and boolean operations for fixed-point types
     */

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct add<tml::fixed::number<INTEGER_T,LHS>,tml::fixed::number<INTEGER_T,RHS>> :
        public tml::function<tml::fixed::number<INTEGER_T,LHS + RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct sub<tml::fixed::number<INTEGER_T,LHS>,tml::fixed::number<INTEGER_T,RHS>> :
        public tml::function<tml::fixed::number<INTEGER_T,LHS - RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct mul<tml::fixed::number<INTEGER_T,LHS>,tml::fixed::number<INTEGER_T,RHS>> :
        public tml::function<tml::fixed::number<INTEGER_T,((LHS * RHS) >> tml::fixed::impl::decimal_bits<INTEGER_T>::value)>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct div<tml::fixed::number<INTEGER_T,LHS>,tml::fixed::number<INTEGER_T,RHS>> :
    public tml::function<tml::fixed::number<INTEGER_T,(LHS << tml::fixed::impl::decimal_bits<INTEGER_T>::value ) / RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct equal<tml::fixed::number<INTEGER_T,LHS>,tml::fixed::number<INTEGER_T,RHS>> :
        public tml::function<tml::boolean<LHS == RHS>>
    {};

    template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
    struct less_than<tml::fixed::number<INTEGER_T,LHS>,tml::fixed::number<INTEGER_T,RHS>> :
        public tml::function<tml::boolean<(LHS < RHS)>>
    {};

    namespace impl
    {
        /*
         * Algebra primitives zero and one for fized-point values
         */
        template<typename INTEGER_T , INTEGER_T V>
        struct zero<tml::fixed::number<INTEGER_T,V>> : public tml::function<tml::fixed::number<INTEGER_T,0>>
        {};

        template<typename INTEGER_T , INTEGER_T V>
        struct one<tml::fixed::number<INTEGER_T,V>> : public tml::function<tml::fixed::number<INTEGER_T,( 1 << tml::fixed::impl::decimal_bits<INTEGER_T>::value )>>
        {};

        /*
         * Sign metafunction implementation for fixed-point types
         */
        template<typename INTEGER_T , INTEGER_T V>
        struct sign<tml::fixed::number<INTEGER_T,V>> : public tml::function<tml::Bool<(V >= 0)>>
        {};



        /*
         * Runtime representation of fixed-point values
         */

        template<typename INTEGER_T , INTEGER_T V>
        struct runtime_representation<tml::fixed::number<INTEGER_T,V>> : public tml::function<double>
        {};

        template<typename INTEGER_T , INTEGER_T V>
        struct to_runtime<tml::fixed::number<INTEGER_T,V>>
        {
            using number = tml::fixed::number<INTEGER_T,V>;

            static tml::runtime_representation<number> execute()
            {
                return V / static_cast<tml::runtime_representation<number>>( 1 << tml::fixed::impl::decimal_bits<INTEGER_T>::value );
            }
        };


        /*
         * tml::to_string override
         */
        template<typename INTEGER_T , INTEGER_T V>
        struct to_string<tml::fixed::number<INTEGER_T,V>>
        {
            operator std::string() const
            {
                return std::to_string( tml::to_runtime<tml::fixed::number<INTEGER_T,V>>() );
            }
        };
        
    
    /*
     * Decimal scientific notation for fixed-point numbers
     */
        
    namespace fixed
    {
        namespace impl
        {
            template<typename INTEGER_T , INTEGER_T mantissa , INTEGER_T exponent>
            struct decimal_sci : public tml::function<tml::eval<tml::mul<tml::pow<tml::fixed::normalize<INTEGER_T,10>,
                                                                              tml::integral_constant<INTEGER_T,exponent>
                                                                             >,
                                                                     tml::fixed::normalize<INTEGER_T,mantissa>
                                                                    >
                                                           >
                                                 >
            {};
        }

        /*
         * Specifies a fixed-point value with underlying integer type INTEGER_T
         * using decimal scientific notation.
         */
        template<typename INTEGER_T , INTEGER_T mantissa , INTEGER_T exponent>
        using decimal_sci = typename tml::fixed::impl::decimal_sci<INTEGER_T,mantissa,exponent>::result;

        /*
         * Specifies a tml::fsingle value with underlying integer type INTEGER_T
         * using decimal scientific notation.
         */
        template<tml::fixed::impl::single_t mantissa , tml::fixed::impl::single_t exponent>
        using single_sci = tml::fixed::decimal_sci<tml::fixed::impl::single_t,mantissa,exponent>;

        /*
         * Specifies a tml::fdouble value with underlying integer type INTEGER_T
         * using decimal scientific notation.
         */
        template<tml::fixed::impl::double_t mantissa , tml::fixed::impl::double_t exponent>
        using double_sci = tml::fixed::decimal_sci<tml::fixed::impl::double_t,mantissa,exponent>;
    } /* tml::fixed namespace */
} /* tml namespace */

#endif	/* FIXED_POINT_HPP */

