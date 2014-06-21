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

#ifndef HIGH_ORDER_FUNCTIONS_HPP
#define	HIGH_ORDER_FUNCTIONS_HPP

#include "list.hpp"
#include "function.hpp"
#include "eval.hpp"
#include "control_structures.hpp"
#include "lambda.hpp"
#include "function_alias_decl.hpp"
#include "placeholders.hpp"
#include "iterator.hpp"

using namespace tml::placeholders;


/*
 * This header declares a set of functional-style high-order metafunctions
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


namespace tml
{
    namespace impl
    {
        /*
         * Applies a given function F to each element of an specified sequence.
         * Returns the sequence of applications.
         * 
         * The list-based implementation has O(1) (Is based on direct variadic-pack expansion),
         * the iterators based has O(n) (Exactly O(distance(begin,end)))
         */
        template<typename... ARGS>
        struct map;
        
        /*
         * List-based sequence map function.
         * 
         * O(1) complexity
         */
        template<typename T , typename... SEQUENCE>
        struct map<T,tml::list<SEQUENCE...>,tml::empty_list> : public tml::function<tml::list<tml::eval<T,SEQUENCE>...>>
        {};
               
        /*
         * Iterators-based sequece map function
         * 
         * NOTE: PASSED is the sequence of elements of the original sequence which have currently passed the filter.
         * This sequence is passed to the next calls to be filled.
         * 
         * O(n) complexity
         */
        template<typename T , typename... PROCESSED ,  typename BEGIN , typename END>
        struct map<T,BEGIN,END,tml::list<PROCESSED...>>
        {
            using result = typename map<T,tml::iterator::next<BEGIN>,
                                          END,
                                          tml::list<PROCESSED...,tml::eval<T,tml::iterator::deref<BEGIN>>>
                                       >::result;
        };
        
        template<typename T , typename... PROCESSED , typename END>
        struct map<T,END,END,tml::list<PROCESSED...>>
        {
            using result = tml::list<PROCESSED...>;
        };
        
        /*
         * Filters a specified sequence, returning a sequence formed with the elements
         * of the original sequence which passed the specified test.
         * 
         * Its complexity is O(n) for both list sequences and iterator sequences.
         */
        template<typename... ARGS>
        struct filter;
        
        
        /*
         * List-based sequence filter implementation (Recursive case)
         * 
         * NOTE: PASSED is the sequence of elements of the original sequence which have currently passed the filter.
         * This sequence is passed to the next calls to be filled.
         */
        template<typename F , typename... PASSED ,  typename HEAD , typename... TAIL>
        struct filter<F,tml::list<HEAD,TAIL...>,tml::list<PASSED...>,tml::empty_list>
        {
            using passed = tml::conditional<tml::eval<F,HEAD>,
                                            tml::list<PASSED...,HEAD>,
                                            tml::list<PASSED...>
                                           >;
            
            using result = typename filter<F,tml::list<TAIL...>,passed>::result;
        };
        
        /*
         * List-based sequence filter implementation (Base case)
         * 
         * NOTE: PASSED is the sequence of elements of the original sequence which have currently passed the filter.
         * In the base case, this sequence is the final result of the function (There are no more elements of the input 
         * sequence to check).
         */
        template<typename F , typename... PASSED>
        struct filter<F,tml::empty_list,tml::list<PASSED...>,tml::empty_list>
        {
            using result = tml::list<PASSED...>;
        };
        
        /*
         * Iterator-based sequence filter implementation (Recursive case)
         * 
         * NOTE: PASSED is the sequence of elements of the original sequence which have currently passed the filter.
         * This sequence is passed to the next calls to be filled.
         */
        template<typename F , typename... PASSED ,  typename BEGIN , typename END>
        struct filter<F,BEGIN,END,tml::list<PASSED...>>
        {
            using passed = tml::conditional<tml::eval<F,tml::iterator::deref<BEGIN>>,
                                            tml::list<PASSED...,tml::iterator::deref<BEGIN>>,
                                            tml::list<PASSED...>
                                           >;
            
            using result = typename filter<F,tml::iterator::next<BEGIN>,END,passed>::result;
        };
        
        /*
         * Iterator-based sequence filter implementation (Base case)
         * 
         * NOTE: PASSED is the sequence of elements of the original sequence which have currently passed the filter.
         * In the base case, this sequence is the final result of the function (There are no more elements of the input 
         * sequence to check).
         */
        template<typename F , typename END , typename... PASSED>
        struct filter<F,END,END,tml::list<PASSED...>>
        {
            using result = tml::list<PASSED...>;
        };
        
        
        /*
         * Folding functions.
         * 
         * Fold functions are functions that process a data structure and build a return value.
         */
        
        template<typename... ARGS>
        struct foldr;
        template<typename... ARGS>
        struct foldl;
        
        /*
         * List-based foldr metafunction
         * 
         * O(n) complexity
         */
        template<typename F , typename STATE , typename HEAD , typename... TAIL>
        struct foldr<F,STATE,tml::list<HEAD,TAIL...>>
        {
            using result = tml::eval<F,HEAD,typename foldr<F,STATE,tml::list<TAIL...>>::result>;
        };
        
        template<typename F , typename STATE>
        struct foldr<F,STATE,tml::empty_list>
        {
            using result = STATE;
        };
        
        
        /*
         * List-based foldl metafunction
         * 
         * O(n) complexity
         */
        template<typename F , typename STATE , typename HEAD , typename... TAIL>
        struct foldl<F,STATE,tml::list<HEAD,TAIL...>>
        {
            using result = typename foldl<F,tml::eval<F,STATE,HEAD>,tml::list<TAIL...>>::result;
        };
        
        template<typename F , typename STATE>
        struct foldl<F,STATE,tml::empty_list>
        {
            using result = STATE;
        };
        
        /*
         * Iterator-based foldr metafunction
         * 
         * O(n) complexity
         */
        template<typename F , typename STATE , typename BEGIN , typename END>
        struct foldr<F,STATE,BEGIN,END>
        {
            using result = tml::eval<F,tml::iterator::deref<BEGIN>,
                                       typename foldr<F,STATE,tml::iterator::next<BEGIN>,END>::result
                                    >;
        };
        
        template<typename F , typename STATE , typename END>
        struct foldr<F,STATE,END,END>
        {
            using result = STATE;
        };
        
        /*
         * Iterator-based foldl metafunction
         * 
         * O(n) complexity
         */
        template<typename F , typename STATE , typename BEGIN , typename END>
        struct foldl<F,STATE,BEGIN,END>
        {
            using result = typename foldl<F,tml::eval<F,STATE,tml::iterator::deref<BEGIN>>,
                                          tml::iterator::next<BEGIN>,
                                          END
                                         >::result;
        };
        
        template<typename F , typename STATE , typename END>
        struct foldl<F,STATE,END,END>
        {
            using result = STATE;
        };
    }
    
    
    /*
     * Applies a given transformation function T to a sequence, returning the sequence 
     * of applications. The argumments of the function could be:
     * 
     * List-based sequence:
     * ====================
     *  - T: The transformation function. It should be a unary function enity.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - T: The transformation function. It should be a unary function enity.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     * 
     * The result is a tml::list filled with the sequence of applications.
     */
    template<typename T , typename... SEQ>
    using map = typename tml::impl::map<T,SEQ...,tml::empty_list>::result; //Functional (Haskell-like) name
    
    template<typename T , typename... SEQ>
    using transform = tml::map<T,SEQ...>;//C++ (STL-like) name
    
    
    
    
    /*
     * Filters the contents of an input sequence based on a boolean predicate filter F.
     * 
     * The parameters are:
     * 
     * List-based sequence:
     * ====================
     *  - F: The filter function. It should be an unary boolean function entity.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - F: The filter function. It should be an unary boolean function entity.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     * 
     * The result is a tml::list filled with the sequence of elements which passed the filter F.
     */
    template<typename F , typename... SEQ>
    using filter = typename tml::impl::filter<F,SEQ...,tml::empty_list>::result; //Functional (Haskell-like) name
    
    template<typename F , typename... SEQ>
    using copy_if = tml::filter<F,SEQ...>;//C++ (STL-like) name
    
    
    /*
     * Haskell-like foldl metafunction.
     * 
     * Processes an input sequence generating a result value through recursive
     * applications of a given composition function F. foldl applies the recursion
     * on the left side of the composition operation.
     * 
     * The argumments of the function could be:
     * 
     * List-based sequence:
     * ====================
     *  - F: The composition function. It should be a binary function enity, where the 
     *       first (left) argumment is the current value (state) of the computation, and the second
     *       is the current element of the sequence.
     *  - STATE: Initial state of the computation.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - F: The composition function. It should be a binary function enity, where the 
     *       first (left) argumment is the current value (state) of the computation, and the second
     *       is the current element of the sequence.
     *  - STATE: Initial state of the computation.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     */
    TURBO_DEFINE_FUNCTION_TALIAS( foldl ); //Functional (Haskell-like) name
    
    
    /*
     * Haskell-like foldr metafunction.
     * 
     * Processes an input sequence generating a result value through recursive
     * applications of a given composition function F. foldr applies the recursion
     * on the right side of the composition operation.
     * 
     * The argumments of the function could be:
     * 
     * List-based sequence:
     * ====================
     *  - F: The composition function. It should be a binary function enity, where the 
     *       second (right) argumment is the current value (state) of the computation, and the first
     *       is the current element of the sequence.
     *  - STATE: Initial state of the computation.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - F: The composition function. It should be a binary function enity, where the 
     *       second (right) argumment is the current value (state) of the computation, and the first
     *       is the current element of the sequence.
     *  - STATE: Initial state of the computation.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     */
    TURBO_DEFINE_FUNCTION_TALIAS( foldr ); //Functional (Haskell-like) name
}

#endif	/* HIGH_ORDER_FUNCTIONS_HPP */

