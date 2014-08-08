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
#include "integral.hpp"
#include "utility.hpp"
#include "integral_lists.hpp"
#include "control_structures.hpp"
#include "to_runtime.hpp"
#include "to_string.hpp"

#include <cmath>


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
         * precission properties nor exception (Overflow/underflow, NaNs, etc) rules.
         * Its only a working floating-point implementation.
         * 
         * Also note that this implementation uses mantissa normalization as expected, but doesn't
         * provide the implicit extra bit of precission of the topmost always-one bit. The mantissa
         * uses a 64 bit unsigned integer and the precission provided is 64 bits. Thats done to make 
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
            
            template<typename F>
            struct highest_set_bit<F,0> : public tml::function<tml::size_t<0>>
            {};
            
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
     * Algebraic functions
     */
    
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
    struct abs<tml::floating::number<S,E,M>> : public tml::function<tml::floating::number<tml::floating::sign_t::positive,E,M>>
    {};
    
    template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
    struct opposite<tml::floating::number<S,E,M>> : 
        public tml::function<tml::floating::number<((bool)S) ? tml::floating::sign_t::negative : tml::floating::sign_t::positive,E,M>>
    {};
    
    
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
     * Runtime and string representations
     */
    namespace impl
    {
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct runtime_representation<tml::floating::number<S,E,M>> : public tml::function<double>
        {};

        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct to_runtime<tml::floating::number<S,E,M>>
        {
            using number    = tml::floating::number<S,E,M>;
            using runtime_t = tml::runtime_representation<number>;
            
            static constexpr runtime_t execute()
            {
                //TODO: compile-time float instead of std::pow()
                return static_cast<runtime_t>( M ) * std::pow( 2.0 , E ) * (((bool)S) ? 1 : -1);
            }
        };
        
        template<tml::floating::sign_t S , tml::floating::exponent_t E , tml::floating::mantissa_t M>
        struct to_string<tml::floating::number<S,E,M>>
        {
            using number    = tml::floating::number<S,E,M>;
            using runtime_t = tml::runtime_representation<number>;

            operator std::string()
            {
                static const std::bitset<32> bits = M;
                std::stringstream ss;

                ss << '[' << (((bool)S) ? '+' : '-') << '|' << E << '|' << bits << "]\n";

                return ss.str();
            }
        };
    }
}

#endif	/* FLOAT_HPP */

