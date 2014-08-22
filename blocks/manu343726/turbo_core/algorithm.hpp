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

#ifndef ALGORITHM_HPP
#define	ALGORITHM_HPP

#include "manu343726/turbo_core/high_order_functions.hpp"
#include "manu343726/turbo_core/list_algorithms.hpp"
#include "manu343726/turbo_core/integral_lists.hpp"
#include "manu343726/turbo_core/algebra.hpp"

/*
 * This header declares a set of functional-style algorithms built upon the
 * high-order metafunctions defined at "high_order_functions.hpp".
 * 
 * Algorithms which work with sequences are dessigned to work with sequences
 * represented by lists (tml::list) and pairs of iterators.
 * 
 * The library provides the same functionality via two naming conventions:
 *  - Functional naming convention: Algorithms have common names used in functional 
 *    programming, such as 'tml::map', 'tml::fold', etc.
 *  
 *  - C++ (STL) naming convention: The names mimic those from the Standard Library
 *    <algoritm> header: 'tml::transform', 'tml::accumulate', etc. 
 * 
 * The implementation uses the functional convention by default.
 */

using tml::placeholders::_1;
using tml::placeholders::_2;

namespace tml
{
    namespace impl
    {
        /*
         * Successively applies a function to a value the specified number of times
         */
        template<typename F , typename STATE , typename BEGIN , typename END , typename ADVANCE>
        struct apply_for
        {
            using next_state   = tml::eval<F,STATE,BEGIN>;
            using next_counter = tml::eval<ADVANCE,BEGIN>;
            
            using result = typename apply_for<F,next_state,next_counter,END,ADVANCE>::result;
            
            TURBO_ENSURE_ALGEBRAIC_EVAL(result);
        };
        
        template<typename F , typename STATE , typename END , typename ADVANCE>
        struct apply_for<F,STATE,END,END,ADVANCE>
        {
            using result = STATE;
            
            TURBO_ENSURE_ALGEBRAIC_EVAL(result);
        };

        
        
        /*
         * Successively applies a function to a value while the value meets a condition
         */
        template<typename F , typename STATE , typename WHILE , typename CONTINUE>
        struct apply_while_impl;
        
        template<typename F , typename STATE , typename WHILE>
        struct apply_while_impl<F,STATE,WHILE,tml::true_type>
        {
            using next_state = tml::eval<F,STATE>;
            using continue_running = tml::eval<WHILE,next_state,STATE>;
            
            using result = typename apply_while_impl<F,next_state,WHILE,continue_running>::result;
            
            TURBO_ENSURE_ALGEBRAIC_EVAL(result);
        };

        template<typename F , typename STATE , typename WHILE>
        struct apply_while_impl<F,STATE,WHILE,tml::false_type>
        {
            using result = STATE;
            
            TURBO_ENSURE_ALGEBRAIC_EVAL(result);
        };
        
        /*
         * Successively applies a function to a value while the value meets a condition
         */
        template<typename F , typename STATE , typename WHILE>
        struct apply_while
        {
            using result = typename apply_while_impl<F,STATE,WHILE,tml::true_type>::result;
            
            TURBO_ENSURE_ALGEBRAIC_EVAL(result);
        };
        
        /*
         * Successively applies a function to a value while the value meets a condition
         */
        template<typename F , typename STATE , typename UNTIL>
        struct apply_until
        {
            using result = typename apply_while<F,STATE,
                                                tml::lambda<tml::placeholders::_1 , tml::placeholders::_2 , tml::logical_not<tml::deval<UNTIL,tml::placeholders::_1,tml::placeholders::_2>>>
                                               >::result;
            
            TURBO_ENSURE_ALGEBRAIC_EVAL(result);
        };
    }
    
    
    
    
    
    /*
     * Returns true if any element of a sequence has some property represented by a 
     * boolean predicate P
     * 
     * The argumments of the function could be:
     * 
     * List-based sequence:
     * ====================
     *  - P: The boolean predicate. It should be a unary boolean function enity.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - P: The boolean predicate. It should be a unary boolean function enity.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     */
    template<typename P , typename... SEQ>
    using any = tml::foldl<tml::lambda<_1,_2 , tml::logical_or<_1,tml::deval<P,_2>>> , tml::false_type , SEQ...>; //Functional (Haskell-like) name)
    
    template<typename P , typename... SEQ>
    using any_of = tml::any<P,SEQ...>; //C++ (STL-ish) name
    
    /*
     * Returns true if all elements of a sequence have some property represented by a 
     * boolean predicate P
     * 
     * The argumments of the function could be:
     * 
     * List-based sequence:
     * ====================
     *  - P: The boolean predicate. It should be a unary boolean function enity.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - P: The boolean predicate. It should be a unary boolean function enity.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     */
    template<typename P , typename... SEQ>
    using all = tml::foldl<tml::lambda<_1,_2 , tml::logical_and<_1,tml::deval<P,_2>>> , tml::true_type , SEQ...>; //Functional (Haskell-like) name)
    
    template<typename P , typename... SEQ>
    using all_of = tml::all<P,SEQ...>; //C++ (STL-ish) name
    
    
    /*
     * Given a value T, returns a tml::list containing N Ts.
     * 
     * Parameters:
     * ===========
     *  - T: The type to be repeated
     *  - N: An unsigned integral value which represents the number of repetitions.
     */
    template<typename T , typename N>
    using repeat = tml::map<tml::lambda<_1 , T>,tml::size_t_range<0,N::value>>;
    
    
    /*
     * Successively applies a function to a value the specified number of times.
     * 
     * Function argumments:
     *  - F: Binary function. Takes the current value and returns the next. The first parameter is the current value, and the second is the current
     *       value of the counter.
     *  - STATE: Initial value.
     *  - BEGIN , END: The number of times the transformation is applied to the value is represented as a range [BEGIN,END), where BEGIN and END are values.
     *  - ADVANCE: Given the current element of the "number-of-times interval" (See above), computes the next element. By default increases the value by 1.
     */
    template<typename F , typename STATE , typename BEGIN , typename END , typename ADVANCE = tml::lambda<tml::placeholders::_1 , tml::add<tml::one<BEGIN>,tml::placeholders::_1>>>
    using apply_for = typename tml::impl::apply_for<F,STATE,BEGIN,END,ADVANCE>::result;
    
    /*
     * Successively applies a function to a value while a certain condition is met.
     * 
     * Function argumments:
     *  - F: Unary function. Takes the current value and returns the next.
     *  - STATE: Initial value.
     *  - WHILE: Binary boolean function. Represents the condition to be met. The first parameter is the current value (Value after current function application), and the second the previous one (Value just after 
     *           current function application).
     *           This allows to easily write convergence conditions. If you only want a property based on the current value, just ignore the second function param.
     */
    template<typename F , typename STATE , typename WHILE>
    using apply_while = typename tml::impl::apply_while<F,STATE,WHILE>::result;
    
    /*
     * Successively applies a function to a value until a certain condition is met.
     * 
     * Function argumments:
     *  - F: Unary function. Takes the current value and returns the next.
     *  - STATE: Initial value.
     *  - UNTIL: Binary boolean function. Represents the condition to be met. The first parameter is the current value (Value after current function application), and the second the previous one (Value just after 
     *           current function application).
     *           This allows to easily write convergence conditions. If you only want a property based on the current value, just ignore the second function param.
     */
    template<typename F , typename STATE , typename UNTIL>
    using apply_until = typename tml::impl::apply_until<F,STATE,UNTIL>::result;
    
}

#endif	/* ALGORITHM_HPP */

