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

#include "high_order_functions.hpp"
#include "list_algorithms.hpp"
#include "integral_lists.hpp"

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
     * Gievn a range and an unary predicate, returns two lists, one with the values evaluated to true 
     * and other with the elements evaluated to false.
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
    
}

#endif	/* ALGORITHM_HPP */

