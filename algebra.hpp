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
#include "chameleon.hpp"
#include "function.hpp"


/*
 * This header declares the set of basic algebra metafunctions. Like other functions, one sould evaluate
 * them to retrieve the result.
 * 
 * Also this header defines a set operators overloading which implement that algebra operations.
 * That operators implement expression templates which compute the desired algebra operation.
 * The result could be extracted via a decltype() aplication, or the macro ALGEVAL(), which is just
 * a shorthand for decltype.
 * 
 * Finally, this header defines some algebra primitives, customizable for different types.
 */

namespace tml
{
    /*
     * Algebra primitives zero and one.
     * sign metafunction.
     */
    namespace impl
    {
        template<typename T>
        struct zero;
        
        template<typename T>
        struct one;
        
        template<typename T>
        struct sign : public tml::not_evaluated_function_chameleon
        {};
    }
    
    namespace func
    {
        /*
         * Returns the zero value of a given numeric type
         */
        template<typename T>
        using zero = tml::impl::zero<T>;
        
        /*
         * Returns the one value of a given numeric type
         */
        template<typename T>
        using one = tml::impl::one<T>;
        
        /*
         * Returns the sign of a given numeric value.
         * Returns tml::true_type if the value is positive, tml::false_type if its negative.
         */
        template<typename T>
        using sign = tml::impl::sign<T>;
    }
    
    /*
     * Returns the zero value of a given numeric type
     */
    template<typename T>
    using zero = typename tml::func::zero<T>::result;
    
    /*
     * Returns the one value of a given numeric type
     */
    template<typename T>
    using one = typename tml::func::one<T>::result;
    
    /*
     * Returns the sign of a given numeric value.
     * Returns tml::true_type if the value is positive, tml::false_type if its negative.
     */
    template<typename T>
    using sign = typename tml::func::sign<T>::result;

    
    /*
     * Returns the same numeric value with a opposite sign.
     */
    template<typename T>
    struct opposite : public tml::not_evaluated_function_chameleon
    {};
    
    /*
     * Returns the absolute value of a given numeric value T.
     */    
    template<typename T>
    struct abs : public tml::not_evaluated_function_chameleon
    {};
    
    
    
    /* Basic algebraic operations */
        
    /*
     * Performs the addition of two values
     */
    template<typename LHS , typename RHS>
    struct add : public tml::not_evaluated_function_chameleon
    {};

    /*
     * Performs the substraction of two values
     */
    template<typename LHS , typename RHS>
    struct sub : public tml::not_evaluated_function_chameleon
    {};

    /*
     * Performs the multiplication of two values
     */
    template<typename LHS , typename RHS>
    struct mul : public tml::not_evaluated_function_chameleon
    {};

    /*
     * Performs the division of two values
     */
    template<typename LHS , typename RHS>
    struct div : public tml::not_evaluated_function_chameleon
    {};
    
    /*
     * Computes the reciprocal (1/N) of a given number N
     */
    template<typename N>
    struct reciprocal : public tml::not_evaluated_function_chameleon
    {};
    
    /*
     * Computes the log10 of a number N
     */
    template<typename T>
    struct log10 : public tml::not_evaluated_function_chameleon
    {};
    
    
    /* Basic logical functions */
    
    /*
     * Performs the addition of two values
     */
    template<typename LHS , typename RHS>
    struct logical_or : public tml::not_evaluated_function_chameleon
    {};

    /*
     * Performs the substraction of two values
     */
    template<typename LHS , typename RHS>
    struct logical_and : public tml::not_evaluated_function_chameleon
    {};

    /*
     * Performs the multiplication of two values
     */
    template<typename OP>
    struct logical_not : public tml::not_evaluated_function_chameleon
    {};
    
    
    /* Comparison functions */
    
    /*
     * Checks if two values are equal.
     * 
     * Returns tml::true_type if the values are equal, returns tml::false_type otherwise.
     */
    template<typename LHS , typename RHS>
    struct equal : public tml::not_evaluated_function_chameleon
    {};
    
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
    struct less_than : public tml::not_evaluated_function_chameleon
    {};
    
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

