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
#include "list.hpp"
#include "enable_if.hpp"
#include "function.hpp"

namespace tml
{
    
    /*
     * This metafunction says if one type of expression has overrided the default
     * behaviour of tml::eval. Every user-defined type which wants to override 
     * tml::eval should be registered specializing this metafunction inheriting
     * from tml::true_type.
     */
    template<typename E>
    struct overrides_eval : public tml::false_type
    {};
    
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
         *  - ARGS: evaluate could be used as a high-order metafunction to evaluate a given
         *             function entity with the specified parameters. This variadic pack is that
         *             set of parameters. The result of the evaluation is the result of evaluating
         *             the functional expresion E with the specified ARGS... arguments.
         *             Note that in this case the argumments are evaluated too (Just for the case they are
         *             functional expressions).
         * 
         *  - SFINAE_FALGS: Parameters used to enable/disable certain specializations.
         * 
         *  Of course this metafunction is a function too, so it stores the result of the evaluation in a 'result' member type.
         */
        template<typename E , typename ARGS , typename SFINAE_FLAGS = tml::sfinae_return>
        struct eval;

        /* This is the most simple case: There are no evaluation parameters (So the expression could be any
         * kind of expression, not just a function) BUT the flag says the expression is not a function.
         * The result of evaluatiing such expression is the expression itself.
         */
        template<typename E>
        struct eval<E,tml::empty_list,
                    tml::sfinae_list<
                                     TURBO_DISABLE_IF(tml::overrides_eval<E>),
                                     TURBO_DISABLE_IF(tml::is_function<E>)
                                    >
                   >
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
        struct eval<F<ARGS...>,tml::empty_list,
                    tml::sfinae_list<
                                     TURBO_DISABLE_IF(tml::overrides_eval<F<ARGS...>>),
                                     TURBO_ENABLE_IF(tml::is_function<F<ARGS...>>)
                                    >
                   > : 
                   public F<typename eval<ARGS,tml::empty_list>::result...> 
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
        struct eval<F<PLACEHOLDERS...> , tml::list<ARG,ARGS...>,
                    tml::sfinae_list<
                                     TURBO_DISABLE_IF(tml::overrides_eval<F<PLACEHOLDERS...>>),
                                     TURBO_ENABLE_IF(tml::is_function<F<PLACEHOLDERS...>>)
                                    >
                   > : 
                   public F<typename eval<ARG,tml::empty_list>::result,
                            typename eval<ARGS,tml::empty_list>::result...
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
     * Also the template wrapper 'tml::lazy' defined in "lazy.hpp" could be used to pass a template
     * without parameters and pass that parameters later when the evaluation is done.
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
    using eval = typename impl::eval<EXPRESSION , tml::list<ARGS...>>::result;
    
}

#endif	/* FUNCTIONAL_HPP */

