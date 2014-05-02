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

#ifndef FUNCTIONAL_HPP
#define	FUNCTIONAL_HPP

#include <type_traits>
#include "function.hpp"

namespace tml
{
    namespace impl
    {
        /*
         * Here we implement the user-side tml::eval<> metafunction. 
         * 
         * The purpose of that metafunction is to evaluate any kind of expression.
         * That serves to evaluate functions and other functional expressions.
         */   


        /*
         * The implementation has three parameters:
         * 
         *  - IS_FUNCTION: A boolean flag which says if the expression passed is a function.
         *    Its used to evaluate the expression correctly (Extracting its 'result' member type
         *    or not).
         * 
         *  - E: The expression to be evaluated.
         * 
         *  - ARGS...: evaluate could be used as a high-order metafunction to evaluate a given
         *             function entity with the specified parameters. This variadic pack is that
         *             set of parameters. The result of the evaluation is the result of evaluating
         *             the functional expresion E with the specified ARGS... arguments.
         *             Note that in this case the argumments are evaluated too (Just for the case they are
         *             functional expressions).
         * 
         *  Of course this metafunction is a function too, so it stores the result of the evaluation in a 'result' member type.
         */
        template<typename E , typename... ARGS>
        struct evaluate_impl;

        /* This is the most simple case: There are no evaluation parameters (So the expression could be any
         * kind of expression, not just a function) BUT the flag says the expression is not a function.
         * The result of evaluatiing such expression is the expression itself.
         */
        template<typename E>
        struct evaluate_impl<E>
        {
            using result = E;
        };
        
        /*
         * This specialization matches the case when the expression passed is a function.
         * The result of the evalutation is just forwarded to the implementation, to reduce
         * template instantation stack usage (Reduce template instantation depth).
         * 
         * So the implementation just inherit the function to get its result.
         * 
         * The parameters are evaluated too (Could be functional expressions) to evaluate the entire
         * expression recursively.
         */
        template<template<typename...> class F , typename... ARGS>
        struct evaluate_impl<F<ARGS...>> : public F<typename evaluate_impl<ARGS>::result...> 
        {};

        /*
         * This is the case when the expression passed is a function, and a set of parameters (At least one) is 
         * passed to evaluate the function with. 
         *
         * Note that the parameters of the function call are evaluated too.
         * 
         * The result is the result of evaluating the function with that parameters.
         */
        template<template<typename...> class F , typename... PLACEHOLDERS , typename ARG , typename... ARGS>
        struct evaluate_impl<F<PLACEHOLDERS...> , ARG , ARGS...> : 
        public F<typename evaluate_impl<ARG>::result,
                 typename evaluate_impl<ARGS>::result...
                >
        {
            static_assert( sizeof...(PLACEHOLDERS) == (1 + sizeof...(ARGS)) , "Wrong number of function call parameters." );  
        };
    }
    
    /*
     * Metafunction to evaluate expressions.
     * 
     * The purpose of this metafunction is to evaluate homogeneously any kind of expression.
     * Also, this could be used as a high-order metafunction to call an specified functional
     * expression with a custom set of parameters.
     * 
     * Note that the parameters of the expression will be ignored during evaluation. 
     * If your intention is to pass the expression to a high order metafunction,
     * you have to fill that parameters even if they will not be used during evaluation.
     * The set of placeholders defined in "placeholders.hpp" could be used for that purpose.
     * 
     * The metafunction has the following parameters:
     *  - E: The expression to be evaluated.
     * 
     *  - ARGS...: evaluate could be used as a high-order metafunction to evaluate a given
     *             function entity with the specified parameters. This variadic pack is that
     *             set of parameters. The result of the evaluation is the result of evaluating
     *             the functional expresion E with the specified ARGS... arguments.
     *             Note that in this case the argumments are evaluated too (Just for the case they are
     *             functional expressions).
     */
    template<typename EXPRESSION , typename... ARGS>
    using eval = typename impl::evaluate_impl<EXPRESSION , ARGS...>::result;
    
}

#endif	/* FUNCTIONAL_HPP */

