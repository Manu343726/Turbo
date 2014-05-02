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

#include "list.hpp"
#include "function.hpp"
#include "eval.hpp"
#include "control_structures.hpp"


/*
 * This header declares a set of functional-style algorithms
 * 
 * Algorithms which work with sequences are dessigned to work with sequences
 * represented by lists (tml::list) and pairs of iterators.
 * NOTE: Iterators are not currently supported
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
         */
        template<typename T , typename... SEQUENCE>
        struct map<T,tml::list<SEQUENCE...>> : public tml::function<tml::list<tml::eval<T,SEQUENCE>...>>
        {};
        
        
        
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
        struct filter<F,tml::list<HEAD,TAIL...>,tml::list<PASSED...>>
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
        struct filter<F,tml::empty_list,tml::list<PASSED...>>
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
        
        
        template<typename F , typename STATE , typename HEAD , typename... TAIL>
        struct foldr<F,STATE,tml::list<HEAD,TAIL...>>
        {
            using result = tml::eval<F,HEAD,foldr<F,STATE,tml::list<TAIL...>>>;
        };
        
        template<typename F , typename STATE>
        struct foldr<F,STATE,tml::empty_list>
        {
            using result = STATE;
        };
        
        
        
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
    
    //Functional name:
    template<typename... ARGS>
    using map = typename tml::impl::map<ARGS...>::result;
    
    //C++ STLish name:
    template<typename... ARGS>
    using transform = tml::map<ARGS...>;
    
    
    
    
    /*
     * Filters the contents of an input sequence based on a boolean predicate filter F.
     * 
     * The parameters are:
     * 
     * List-based sequence:
     * ====================
     *  - F: The filter function. It should be a unary boolean function entity.
     *  - SEQ: The sequence, represented as a tml::list.
     * 
     * Iterators-based sequence:
     * =========================
     *  - F: The filter function. It should be a unary boolean function entity.
     *  - BEGIN: An iterator pointing to the beginning of the input sequence.
     *  - END: An iterator pointing to the end of the input sequence. Note sequences represented
     *    as pairs of iterators are sequences of the interval [BEGIN,END).
     * 
     * The result is a tml::list filled with the sequence of elements which passed the filter F.
     */
    
    //Functional name:
    template<typename... ARGS>
    using filter = typename tml::impl::filter<ARGS...,tml::empty_list>::result;
    
    //C++ STLish name:
    template<typename... ARGS>
    using copy_if = tml::filter<ARGS...>;
    
    
    
    template<typename... ARGS>
    using foldl = typename tml::impl::foldl<ARGS...>::result;
    
    template<typename... ARGS>
    using foldr = typename tml::impl::foldr<ARGS...>::result;
    
    
}

#endif	/* ALGORITHM_HPP */

