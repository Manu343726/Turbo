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

#ifndef ALGREGRA_HPP
#define	ALGREGRA_HPP

#include "function_alias_decl.hpp"


/*
 * This header declares the set of basic algebra metafunctions. Like other functions, one sould evaluate
 * them to retrieve the result.
 * 
 * Also this header defines a set operators overloading which implement that algebra operations.
 * That operators implement expression templates which compute the desired algebra operation.
 * The result could be extracted via a decltype() aplication, or the macro ALGEVAL(), which is just
 * a shorthand for decltype.
 */

namespace tml
{
    /* Basic algebraic operations */
        
    /*
     * Performs the addition of two values
     */
    template<typename LHS , typename RHS>
    struct add
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement add for your custom datatypes" );
    };

    /*
     * Performs the substraction of two values
     */
    template<typename LHS , typename RHS>
    struct sub
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement sub for your custom datatypes" );
    };

    /*
     * Performs the multiplication of two values
     */
    template<typename LHS , typename RHS>
    struct mul
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement mul for your custom datatypes" );
    };

    /*
     * Performs the division of two values
     */
    template<typename LHS , typename RHS>
    struct div
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement div for your custom datatypes" );
    };
    
    
    /* Basic logical functions */
    
    /*
     * Performs the addition of two values
     */
    template<typename LHS , typename RHS>
    struct logical_or
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement logical_or for your custom datatypes" );
    };

    /*
     * Performs the substraction of two values
     */
    template<typename LHS , typename RHS>
    struct logical_and
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement logical_and for your custom datatypes" );
    };

    /*
     * Performs the multiplication of two values
     */
    template<typename OP>
    struct logical_not
    {
        static_assert( sizeof(OP) != sizeof(OP) , "You should implement logical_not for your custom datatypes" );
    };
    
    
    /* Comparison functions */
    
    /*
     * Checks if two values are equal.
     * 
     * Returns tml::true_type if the values are equal, returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct equal
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement equal for your custom datatypes");
    };
    
    /*
     * Checks if two values are not equal.
     * 
     * Returns tml::true_type if the values are not equal, returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct not_equal : public tml::logical_not<typename tml::equal<LHS,RHS>::result>
    {};
    
    /*
     * Checks if a value is lesser than other.
     * 
     * Returns tml::true_type if the first (left) argumment is lesser than the other, 
     * returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct less_than
    {
        static_assert( sizeof(LHS) != sizeof(LHS) , "You should implement less_than for your custom datatypes");
    };
    
    /*
     * Checks if a value is less or equal than other.
     * 
     * Returns tml::true_type if the first (left) value is less or equal than the other, 
     * returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct less_or_equal : public tml::logical_or<typename tml::less_than<LHS,RHS>::result,
                                                  typename tml::equal<LHS,RHS>::result>
    {};
    
    /*
     * Checks if a value is bigger than other.
     * 
     * Returns tml::true_type if the first (left) argumment is bigger than the other,
     * returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct bigger_than : public tml::less_than<RHS,LHS>
    {};
    
    /*
     * Checks if one value is bigger or equal than the other.
     * 
     * Returns tml::true_type if the first (left) argumment is bigger or equal than the other,
     * returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct bigger_or_equal : public tml::less_or_equal<RHS,LHS>
    {};
}

#endif	/* ALGREGRA_HPP */

