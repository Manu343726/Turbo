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

#ifndef FLOAT_HPP
#define	FLOAT_HPP

#include "algebra.hpp"
#include "math.hpp"
#include "integral.hpp"
#include "utility.hpp"
#include "integral_lists.hpp"
#include "control_structures.hpp"
#include "to_runtime.hpp"
#include "to_string.hpp"

#include <cmath>
#include <bitset>
#include <array>

namespace tml
{
    namespace floating
    {
        /*
         * Standard types for floating-point parts:
         * 
         * - Boolean flag as sign (Positive = true , Negative = false ).
         * - 32 bit signed integer exponent.
         * - 64 bit unsigned integer mantissa (Always normalized, but no implicit topmost 1).
         * 
         * Not that this impementation doesn't fullfill the IEEE754 ISO Standard, neither for
         * precision properties nor exception (Overflow/underflow, NaNs, etc) rules.
         * Its only a working floating-point implementation.
         * 
         * Also note that this implementation uses mantissa normalization as expected, but doesn't
         * provide the implicit extra bit of precision of the topmost always-one bit. The mantissa
         * uses a 64 bit unsigned integer and the precision provided is 64 bits. Thats done to make 
         * the implementation more clear (No implicit things) and avoids some denormalizations needed
         * to perform arithmetic operations.
         */
        
        /*
         * Floating-point number sign type.
         */
        enum class sign_t : bool { positive = true , negative = false };
        
        /*
         * Type used to represent floating-point numbers exponent
         */
        using exponent_t = std::int16_t;
        
        /*
         * Type used to represent floating-point numbers mantissa
         */
        using mantissa_t = std::uint32_t;
        
        /*
         * Index of the most significant bit of a mantissa
         */
        using mantissa_msb = tml::size_t<tml::util::sizeof_bits<mantissa_t>::value - 1>;
        
        
        /*
         * Compile-time floating-point value
         */
        template<sign_t S , exponent_t E , mantissa_t M>
        struct number
        {
            static constexpr const sign_t     sign     = S;
            static constexpr const exponent_t exponent = E;
            static constexpr const mantissa_t mantissa = M;
        };
    
        
        
        namespace impl
        {
            /*
             * Returns the index of the top 1 bit of the mantissa of a floating-point number F.
             */
            template<typename F , std::size_t i = tml::floating::mantissa_msb::value>
            struct highest_set_bit
            {
                using result = tml::size_t<(F::mantissa & ( 1U << i)) ? i :
                                            highest_set_bit<F,i-1>::result::value
                                          >;
            };
            
            /*
             * Base case
             */
            template<typename F>
            struct highest_set_bit<F,0>
            {
                using result = tml::size_t<0>;
            };
            
            /*
             * Spcialization for zero
             */
            template<sign_t S , exponent_t E , std::size_t i>
            struct highest_set_bit<tml::floating::number<S,E,0>,i>
            {
                using result = tml::size_t<0>;
            };
            
            /*
             * Shifts a floating-point mantissa adjusting the exponent accordingly. 
             * A positive shift results in a leftshift of the mantissa and an increment of the exponent.
             * A negative shift results in a rightshift of the mantissa and a decrement of the exponent.
             */
            template<typename F , typename COUNT>
            struct shift
            {
                static constexpr const mantissa_t mantissa = (COUNT::value >= 0) ? (F::mantissa << COUNT::value) :
                                                                                   (F::mantissa >> -COUNT::value);
                
                static constexpr const exponent_t exponent = F::exponent - COUNT::value;
                
                using result = tml::floating::number<F::sign,exponent,mantissa>;
            };
            
            /*
             * Normalizes a floating-point number to the specified mantissa bit (MSB by default)
             */
            template<typename F , typename BIT_INDEX = tml::floating::mantissa_msb>
            struct normalize
            {
                static constexpr const std::size_t hsb      = highest_set_bit<F>::result::value;
                static constexpr const int         shift    = BIT_INDEX::value - hsb;
                
                static_assert( hsb   <= tml::floating::mantissa_msb::value , "HSB Overflow" );
                static_assert( shift <= (int)tml::floating::mantissa_msb::value , "Overflow" );
                
                using result = typename tml::floating::impl::shift<F,tml::Int<shift>>::result;
            };
        }
        
        namespace func
        {
            /*
             * Returns the index of the top 1 bit of the mantissa of a floating-point number F.
             */
            template<typename F>
            using highest_set_bit = tml::floating::impl::highest_set_bit<F>;
            
            /*
             * Shifts a floating-point mantissa adjusting the exponent accordingly. 
             * A positive shift results in a leftshift of the mantissa and an increment of the exponent.
             * A positive shift results in a rightshift of the mantissa and a decrement of the exponent.
             */
            template<typename F , typename COUNT>
            using shift = tml::floating::impl::shift<F,COUNT>;
            
            /*
             * Normalizes a floating-point number to the specified mantissa bit (MSB by default)
             */
            template<typename F , typename BIT_INDEX = tml::floating::mantissa_msb>
            using normalize = tml::floating::impl::normalize<F,BIT_INDEX>;
        }
        
        /*
         * Returns the index of the top 1 bit of the mantissa of a floating-point number F.
         */
        template<typename F>
        using highest_set_bit = typename tml::floating::func::highest_set_bit<F>::result;
        
        /*
         * Shifts a floating-point mantissa adjusting the exponent accordingly. 
         * A positive shift results in a leftshift of the mantissa and an increment of the exponent.
         * A positive shift results in a rightshift of the mantissa and a decrement of the exponent.
         */
        template<typename F , typename COUNT>
        using shift = typename tml::floating::func::shift<F,COUNT>::result;

        /*
         * Normalizes a floating-point number to the specified mantissa bit (MSB by default)
         */
        template<typename F , typename BIT_INDEX = tml::floating::mantissa_msb>
        using normalize = typename tml::floating::func::normalize<F,BIT_INDEX>::result;
    }
    
    /*
     * Numeric primitives
     */
    namespace impl
    {
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct zero<tml::floating::number<S,E,M>> : public tml::function<tml::floating::number<tml::floating::sign_t::positive,1,0x00000000>>
        {};
        
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct one<tml::floating::number<S,E,M>> : public tml::function<tml::floating::number<tml::floating::sign_t::positive,-31,0x80000000>>
        {};
        
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct sign<tml::floating::number<S,E,M>> : public tml::function<tml::boolean<(bool)S>>
        {};
    }
    
    /*
     * Comparison functions
     */
    
    template<tml::floating::sign_t S_LHS , tml::floating::exponent_t E_LHS , tml::floating::mantissa_t M_LHS ,
             tml::floating::sign_t S_RHS , tml::floating::exponent_t E_RHS , tml::floating::mantissa_t M_RHS>
    struct equal<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::floating::number<S_RHS,E_RHS,M_RHS>>
    {
        using result = tml::boolean<S_LHS == S_RHS && E_LHS == E_RHS && M_LHS == M_RHS>;
    };
    
    template<tml::floating::sign_t S_LHS , tml::floating::exponent_t E_LHS , tml::floating::mantissa_t M_LHS ,
             tml::floating::sign_t S_RHS , tml::floating::exponent_t E_RHS , tml::floating::mantissa_t M_RHS>
    struct less_than<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::floating::number<S_RHS,E_RHS,M_RHS>>
    {
        static constexpr const bool unsigned_result = E_LHS < E_RHS || ( E_LHS == E_RHS && M_LHS < M_RHS );
        
        using result = tml::Bool<(S_LHS == S_RHS)              ? unsigned_result :
                                 ((bool)S_LHS && !(bool)S_RHS) ? false : true>;
    };
    
    
    /*
     * Algebraic functions
     */
    
    /*
     * Abs: Just put a positive sign. Easy, right?
     */
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
    struct abs<tml::floating::number<S,E,M>> : public tml::function<tml::floating::number<tml::floating::sign_t::positive,E,M>>
    {};
    
    /*
     * Opposite number: Put the opposite sign.
     */
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
    struct opposite<tml::floating::number<S,E,M>> : 
        public tml::function<tml::floating::number<((bool)S) ? tml::floating::sign_t::negative : tml::floating::sign_t::positive,E,M>>
    {};
    
    /*
     * Addition floating-point vs floating-point
     */
    template<tml::floating::sign_t S_LHS , tml::floating::exponent_t E_LHS , tml::floating::mantissa_t M_LHS ,
             tml::floating::sign_t S_RHS , tml::floating::exponent_t E_RHS , tml::floating::mantissa_t M_RHS>
    struct add<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::floating::number<S_RHS,E_RHS,M_RHS>>
    {
        /*
         * Denormalize the numbers to be addable always (Addition of normalized numbers may result in mantissa overflow)
         */
        using dlhs = tml::floating::shift<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::Int<-1>>; //Mantissa one bit to the right, to prevent overflow
        using drhs = tml::floating::shift<tml::floating::number<S_RHS,E_RHS,M_RHS>,tml::Int<-1>>; //Mantissa one bit to the right, to prevent overflow
        
        static constexpr const int diff = E_LHS - E_RHS; //Precission diff between the two numbers
        static constexpr const tml::floating::exponent_t result_exponent = (diff >= 0) ? dlhs::exponent : drhs::exponent; //Exponent of the adjusted numbers.
        
        using denorm_lhs = tml::lconditional<tml::Bool<(diff < 0)>,                                      //If LHS is the lesser
                                             tml::lazy<tml::floating::func::shift,dlhs,tml::Int<diff>>,  //Adjust LHS to RHS
                                             dlhs>;                                                      //else nothing (RHS is adjusted to LHS bellow)
        
        using denorm_rhs = tml::lconditional<tml::Bool<(diff >= 0)>,                                     //If RHS is the lesser
                                             tml::lazy<tml::floating::func::shift,drhs,tml::Int<-diff>>, //Adjust RHS to LHS
                                             drhs>;                                                      //else nothing (LHS is adjusted to RHS above)

        
        /*
         * Addition logic
         */
        template<tml::floating::mantissa_t m1 , tml::floating::mantissa_t m2 , tml::floating::sign_t sign1 , tml::floating::sign_t sign2>
        struct adder;
        
        /*
         * Addition of two numbers of the same sign: Just add values and return with the same sign.
         */
        template<tml::floating::mantissa_t m1 , tml::floating::mantissa_t m2 , tml::floating::sign_t sign>
        struct adder<m1,m2 , sign , sign> : public tml::function<tml::floating::number<sign,result_exponent,m1+m2>>
        {};
        
        /*
         * Addition of two numbers, the first positive and the second negative.
         * This operation is really a substraction between two numbers:
         * 
         *  - If the first is bigger, the result is first-second, with positive sign.
         *  - If the second is bigger, the result is second-first, with negative sign.
         */
        template<tml::floating::mantissa_t m1 , tml::floating::mantissa_t m2>
        struct adder<m1,m2 , tml::floating::sign_t::positive , tml::floating::sign_t::negative>
        {
            static constexpr const tml::floating::mantissa_t mantissa = (m1 >= m2) ? (m1 - m2) : (m2 - m1);
            static constexpr const auto                      sign     = (tml::floating::sign_t)(m1 >= m2);
            
            using result = tml::floating::number<sign,result_exponent,mantissa>;
        };
        
        /*
         * Addition of two numbers, the first negative and the second positive.
         * This operation is really a (conmuted) substraction between two numbers:
         * 
         *  - If the first is bigger, the result is second-first, with negative sign.
         *  - If the second is bigger, the result is first-second, with positive sign.
         */
        template<tml::floating::mantissa_t m1 , tml::floating::mantissa_t m2>
        struct adder<m1,m2 , tml::floating::sign_t::negative , tml::floating::sign_t::positive>
        {
            static constexpr const tml::floating::mantissa_t mantissa = (m1 >= m2) ? (m2 - m1) : (m1 - m2);
            static constexpr const auto                      sign     = (tml::floating::sign_t)(m1 < m2);
            
            using result = tml::floating::number<sign,result_exponent,mantissa>;
        };
        
        //Remember to normalize the result
        using result = tml::floating::normalize<typename adder<denorm_lhs::mantissa,denorm_rhs::mantissa,S_LHS,S_RHS>::result>;
    };
    
    /*
     * Substraction floating-point vs floating-point: Add the first with the opposite of the second
     */
    template<tml::floating::sign_t S_LHS , tml::floating::exponent_t E_LHS , tml::floating::mantissa_t M_LHS ,
             tml::floating::sign_t S_RHS , tml::floating::exponent_t E_RHS , tml::floating::mantissa_t M_RHS>
    struct sub<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::floating::number<S_RHS,E_RHS,M_RHS>> : 
                public tml::function<tml::eval<tml::add<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::opposite<tml::floating::number<S_RHS,E_RHS,M_RHS>>>>>
    {};
    
    
    /*
     * Multiplication floating-point vs floating-point
     */
    template<tml::floating::sign_t S_LHS , tml::floating::exponent_t E_LHS , tml::floating::mantissa_t M_LHS ,
             tml::floating::sign_t S_RHS , tml::floating::exponent_t E_RHS , tml::floating::mantissa_t M_RHS>
    struct mul<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::floating::number<S_RHS,E_RHS,M_RHS>>
    {
        static constexpr const std::uint64_t mantissa_full = (std::uint64_t)M_LHS * (std::uint64_t)M_RHS; //Full multiply
        static constexpr const std::uint32_t mantissa      = mantissa_full >> 32;                         //Readjust mantissa to 32 bit precision 
        static constexpr const auto          exponent      = E_LHS+E_RHS + 32;                            //+32 to compensate bias (Else the exponent would be doubly biased)
        static constexpr const auto          sign          = (tml::floating::sign_t)(!((bool)S_LHS ^ (bool)S_RHS)); 
        
        using result = tml::floating::normalize<tml::floating::number<sign,exponent,mantissa>>;
    };
    
    
    /*
     * Reciprocal: Compute 1/X, where X is a floating-point number, using a Newton-Raphson 
     * approximation of 32 / 4 iterations.
     */
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
    struct reciprocal<tml::floating::number<S,E,M>>
    {
        using N   = tml::floating::number<S,E,M>;
        using one = tml::one<tml::floating::number<__,__,__>>; //Please Clang, be a good friend and memoize this. Thanks
        using two = tml::floating::number<tml::floating::sign_t::positive,-30,0x80000000>; //This too
        
        /*
         * First compute a 1/X guess, say mantissa integer division between 1 and X:
         */
        
        using guess_divisor = tml::floating::normalize<N,tml::size_t<15>>; //Use a divisor with half precision
        
        /*
         * Note about the exponent: Its the opposite since the number was inversed, and - 31 to prevent double bias
         */
        using guess         = tml::floating::number<S, -guess_divisor::exponent - 31 , M / guess_divisor::mantissa>; 
        
        
        /*
         * Now execute a Newton-Raphson aproximation starting with that guess:
         * 
         * f(x)  = 1 / x - N 
         * f'(x) = -1 / x^2
         *  
         *                f(Xn)               1/Xn - N
         * Xn+1 = Xn - _____________ = Xn - ____________ = 2*Xn - N(Xn)^2 = Xn * (2 - N * Xn)
         *                f'(Xn)              -1/(Xn)^2
         *           
         */
        
        /*
         * Configuration (Number of iterations): Currently a quarter of the mantissa precision. 
         */
        using begin = tml::integral_forward_iterators::make_size_t<0>;
        using end   = tml::integral_forward_iterators::make_size_t<tml::util::sizeof_bits<tml::floating::mantissa_t>::value / 4>;
        
        using result = tml::floating::normalize<tml::foldl<tml::lambda<_1,_2 , tml::mul<_1,tml::sub<two,tml::mul<N,_1>>> > , guess , begin , end>>;
    };
    
    
    
    /*
     * Division floating-point vs floating-point: Compute A/B as A * 1/B
     */
    template<tml::floating::sign_t S_LHS , tml::floating::exponent_t E_LHS , tml::floating::mantissa_t M_LHS ,
             tml::floating::sign_t S_RHS , tml::floating::exponent_t E_RHS , tml::floating::mantissa_t M_RHS>
    struct div<tml::floating::number<S_LHS,E_LHS,M_LHS>,tml::floating::number<S_RHS,E_RHS,M_RHS>>
    {
        using result = tml::eval<tml::mul<tml::floating::number<S_LHS,E_LHS,M_LHS>,
                                          tml::reciprocal<tml::floating::number<S_RHS,E_RHS,M_RHS>>
                                         >
                                >;
    };
    
    
    /*
     * Runtime and string representations
     */
    namespace impl
    {
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct runtime_representation<tml::floating::number<S,E,M>> : public tml::function<double>
        {};

        /*
         * Note that the result of tml::to_runtime() should be aviable (computed) at compile-time.
         * In this case, this "to double conversion" should be done at compile-time, to be sure the double values
         * are aviable at compile time and properly placed on the program binary. Hence the constexpr pow2() function.
         * 
         * Having a working floating-point library which does all the computations at compile-time, but needs to do
         * std::pow(2.0,e) at runtime only to get those results has no sense at all...
         */
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct to_runtime<tml::floating::number<S,E,M>>
        {
            static constexpr double pow2(std::int16_t e)
            {
                return e == 0 ? 1. :
                        e > 0 ? 2. * pow2(std::int16_t(e - 1)) :
                                0.5 * pow2(std::int16_t(e + 1));
            }
            
            static constexpr double execute()
            {
                return (((bool)S) ? 1 : -1) * pow2(E) * (double)M;
            }
        };
        
        
        /*
         * to_runtime() specialization for arrays of doubles
         */
        
        template<tml::floating::sign_t... Ss , tml::floating::exponent_t... Es , tml::floating::mantissa_t... Ms>
        struct runtime_representation<tml::list<tml::floating::number<Ss,Es,Ms>...>> : public tml::function<std::array<double,sizeof...(Ss)>>
        {};
        
        template<tml::floating::sign_t... Ss , tml::floating::exponent_t... Es , tml::floating::mantissa_t... Ms>
        struct to_runtime<tml::list<tml::floating::number<Ss,Es,Ms>...>>
        {
            static constexpr const std::array<double,sizeof...(Ss)> array = { tml::to_runtime<tml::floating::number<Ss,Es,Ms>>()... };
            
            static constexpr const std::array<double,sizeof...(Ss)>& execute()
            {                
                return array;
            }
        };
        template<tml::floating::sign_t... Ss , tml::floating::exponent_t... Es , tml::floating::mantissa_t... Ms>
        constexpr const std::array<double,sizeof...(Ss)> to_runtime<tml::list<tml::floating::number<Ss,Es,Ms>...>>::array; //Damn linker...
        
        
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct to_string<tml::floating::number<S,E,M>>
        {
            using number    = tml::floating::number<S,E,M>;
            using runtime_t = tml::runtime_representation<number>;

            operator std::string()
            {
                /*
                 * tml::to_string() prints the internal data of the floating-point number, as:
                 * 
                 *     [sign|exponent|mantissa]
                 * 
                 * Leading to something like:
                 * 
                 *     [+|-21|10000100001000011111010]
                 * 
                 * Which helps a lot to debug this thing.
                 */
                static const std::bitset<tml::util::sizeof_bits<tml::floating::mantissa_t>::value> bits = M;
                std::stringstream ss;

                ss << '[' << (((bool)S) ? '+' : '-') << '|' << E << '|' << bits << "]";

                return ss.str();
            }
        };
    }
    
    /*
     * tml::pow (From math.hpp) specialization for floating-point base and integral exponent
     */
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M , typename T , T V>
    struct pow<tml::floating::number<S,E,M>,tml::integral_constant<T,V>>
    {
        using base = tml::floating::number<S,E,M>;
        
        using result = tml::eval<tml::mul<base,tml::pow<base,tml::integral_constant<T,V-1>>>>;
    };
    
    /*
     * Base case
     */
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M , typename T>
    struct pow<tml::floating::number<S,E,M>,tml::integral_constant<T,0>>
    {
        using result = tml::one<tml::floating::number<__,__,__>>;
    };
    
    
    
    namespace floating
    {
        namespace impl
        {
            template<std::int64_t mantissa , sign_t S = (sign_t)(mantissa >= 0)>
            struct integer
            {
                using m   = tml::floating::number<S,0,static_cast<mantissa_t>((mantissa >= 0) ? mantissa : -mantissa)>;
                using hsb = tml::floating::highest_set_bit<m>;
                static constexpr const exponent_t exp = hsb::value - 31;
                
                using result = tml::floating::number<S,exp,(m::mantissa << (31 - hsb::value))>; //Note the number is normalized
            };
        }
        
        /*
         * An alias suposed to instance integer floating-point values.
         */
        template<std::int64_t mantissa , sign_t S = (sign_t)(mantissa >= 0)>
        using integer = typename tml::floating::impl::integer<mantissa>::result;
        
        namespace impl
        {
            /*
             * Given the integral part of a number (NUMBER) and the decimal places of the number (DECIMALS),
             * the number is computed as:
             * 
             *     number = NUMBER + decimal_part
             * 
             * Where the decimal part is the value of the decimal values:
             * 
             *     decimal_part = DECIMALS / (10^decimal_places_count)
             * 
             * Finally the number of decimal places passed is computed (Counted) as log10(DECIMALS) + 1
             */
            template<std::int64_t NUMBER , std::uint64_t DECIMALS>
            struct decimal
            {   
                using number = tml::floating::integer<NUMBER>;
                
                using decimals_i = tml::integral_constant<std::uint64_t,DECIMALS>;
                using decimals_f = tml::floating::integer<DECIMALS>;
                using digits     = tml::eval<tml::add<tml::log10<decimals_i>,tml::one<decimals_i>>>;
                using divisor    = tml::eval<tml::pow<tml::floating::integer<10>,digits>>;
                
                using decimals_value = tml::eval<tml::div<decimals_f,divisor>>;
                
                using result = tml::eval<tml::add<number,decimals_value>>;
            };
        }
        
        /*
         * An alias suposed to return decimal floating-point values.
         */
        template<std::int64_t NUMBER , std::uint64_t DECIMALS>
        using decimal = typename tml::floating::impl::decimal<NUMBER,DECIMALS>::result;
    }
}

#endif	/* FLOAT_HPP */

