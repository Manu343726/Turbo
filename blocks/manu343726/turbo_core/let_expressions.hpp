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

#ifndef LET_HPP
#define LET_HPP

#include "manu343726/turbo_core/eval.hpp"
#include "manu343726/turbo_core/list.hpp"
#include "manu343726/turbo_core/list_algorithms.hpp"


/*
 * Haskell-like let expressions.
 * 
 * Based on http://www.sciencedirect.com/science/article/pii/S157106611100185X
 */

namespace tml
{
    namespace impl
    {
        /*
         * Internal namespace holds the implementation of the tml::let metafunction.
         * 
         * tml::let has three parameters:
         *    NAME: The variable (name) the value will be binded with in the expression.
         *    VALUE: The value to be binded.
         *    EXPRESSION: The functional expression which used the NAME variable.
         *                This expression could have any number of parameters.
         * 
         * The implementation is divided in two phases: High-level and low-level:
         *  - The high-level phase is in charge of taking the let parameters
         *    and passing them to the low-level phase. 
         * 
         *  - The low-level phase selects the different substitution actions to be 
         *    performed depending on the category of the let parameters. 
         *    For example, it covers the cases when the expression is not a functional
         *    expression (Thus there is no NAME to bind into the expression), and general
         *    case when the expression passed is a functional expression.
         * 
         *    The only case which the low-level phase doesn't cover is the case when the
         *    parameters of the functional expression passed matches the binding name. That
         *    case is covered in the high-level phase. In the other cases, the high-level phase
         *    just forwards the parameters to the low-level phase. 
         */
        
        
        /*
         * Forward declaration of the high-level phase.
         */
        template<typename NAME , typename VALUE , typename EXPRESSION>
        struct let_impl_high;
        
        /*
         * Forward declaration of the low-level phase.
         */
        template<typename NAME , typename VALUE , typename EXPRESSION>
        struct let_impl_low;
        
        
        /* Low level phase: */
        
        /*
         * Low-level phase. The expression passed is a functional expression. 
         * We apply let reursively on the parameters of the expression (Could be functional
         * expressions too). 
         */
        template<typename NAME , typename VALUE , template<typename...> class EXPRESSION , typename... PARAMETERS>
        struct let_impl_low<NAME,VALUE,EXPRESSION<PARAMETERS...>> :
        public tml::function<EXPRESSION<typename let_impl_high<NAME,VALUE,PARAMETERS>::result...>> 
        {
            //static_assert( sizeof(NAME) != sizeof(NAME) , "Instanced" );
        };

        template<typename NAME , typename VALUE , typename E , typename... PARAMETERS>
        struct let_impl_low<NAME,VALUE,E(PARAMETERS...)> :
        public tml::function<tml::delayed_eval<typename let_impl_high<NAME,VALUE,E>::result,typename let_impl_high<NAME,VALUE,PARAMETERS>::result...>> 
        {
            //static_assert( sizeof(NAME) != sizeof(NAME) , "Instanced" );
        };

        /*
         * Special case for lambda bodies expressions.
         * 
         * Lambda bodies with evaluating expressions such as 'tml::eval<F,_1>' doesn't work because the placeholder is substituted after the 
         * expression evaluation.
         * 
         * The library provides the template tml::delayed_eval<F,ARGS...> for that purpose: It holds a functional expression reevaluation
         * with parameters that may be placeholders. When doing let on such template, tml::let substitutes the letted placeholders with its
         * values, and the tml::delayed_eval template with tml::eval.
         * 
         * For that purpose, the low_level phase of tml::let is overrided.
         */
        template<typename NAME , typename VALUE , template<typename...> class EXPRESSION , typename... PARAMETERS>
        struct let_impl_low<NAME,VALUE,tml::lazy<EXPRESSION,PARAMETERS...>> :
        public tml::function<tml::lazy<EXPRESSION,typename let_impl_high<NAME,VALUE,PARAMETERS>::result...>> 
        {
            //static_assert( sizeof(NAME) != sizeof(NAME) , "Instanced" );
        };
        
        /*
         * General case of the low-level phase: The expression is not a functional expression,
         * so there are no variables to bind a value with.
         * The result is the expression itself.
         */
        template<typename NAME , typename VALUE , typename EXPRESSION>
        struct let_impl_low : public tml::function<EXPRESSION> 
        {
            /* Debug trace (DISABLED) */
            //static_assert( sizeof(VALUE) != sizeof(VALUE) , "LOW_PHASE: Simple expression case instanced" );
        };
        
        
        /* High level phase: */
        
        /*
         * General case of the high-level phase: Just parameter forwarding to the 
         * second phase.
         */
        template<typename NAME , typename VALUE , typename EXPRESSION>
        struct let_impl_high : let_impl_low<NAME,VALUE,EXPRESSION> 
        {
            /* Debug trace (DISABLED) */
            //static_assert( sizeof(NAME) != sizeof(NAME) , "HIGH_PHASE: LOW_PHASE call" );
        };
        
        
        /*
         * The variable to be binded and the expression are the same: Apply the
         * substitution of the value:
         */
        template<typename NAME , typename VALUE>
        struct let_impl_high<NAME,VALUE,NAME> : public tml::function<VALUE> 
        {
            /* Debug trace (DISABLED) */
            //static_assert( sizeof(NAME) != sizeof(NAME) , "HIGH_PHASE: Variable --> Name binding specialization instanced" );    
        };
    }
    
    /*
     * Haskell-like let expression
     * 
     * Binds a value to a named variable in the specified expression.
     * This metafunction has the following three parameters:
     *  - NAME: The name (variable) to bind the value with.
     *  - VALUE: The value to be binded.
     *  - EXPRESSION: The expression where the substitution will be applied.
     * 
     * The result of this metafunction is a copy of the passed expression where 
     * the ocurences of the named variable have been substituted with the value.
     * 
     * NOTE: Explicit "typename ::result" construct is used instead of tml::eval
     *       here because the evaluation process chacks for function signature
     *       (See 'tml::impl::is_function' trait in "functional.hpp"), which instanttiates
     *       the expression. The problem is because the use of variables and/or placeholders
     *       could lead to instantation errors in metafunctions which expect some properties
     *       (Being a function, for example) on its parameters. Of course variables and 
     *       and placeholders doesn't has that properties (Are just empty types), so when such
     *       expressions are instantiated with placeholders the instantation fails.
     *       Thats the reason to avoid tml::let here, a context (tml::let) where the usage of
     *       placeholders and variables is common.
     */
    template<typename NAME , typename VALUE , typename EXPRESSION>
    using let = typename impl::let_impl_high<NAME,VALUE,EXPRESSION>::result;
    //PREFER N-ARY LET BELLOW
    
    
    
    namespace impl
    {            

        template<typename Pairs, typename Body>
        struct multi_let_currifier_process;

        template<template<typename, typename> class pair, typename Var, typename Val, typename... Tail, typename Body>
        struct multi_let_currifier_process<tml::list<pair<Var,Val>, Tail...>, Body>
        {
            using body = typename impl::let_impl_high<Var,Val,Body>::result;

            using result = typename multi_let_currifier_process<tml::list<Tail...>, body>::result;
        };
        
        template<typename Body>
        struct multi_let_currifier_process<tml::empty_list, Body>
        {
            using result = Body;
        };

        template<typename... ARGS>
        struct multi_let_currifier
        {
            template<typename Var, typename Val>
            struct pair {};

            template<typename _ARGS, typename PAIRS>
            struct parse_args;

            template<typename Var, typename Val, typename... TAIL,
                     typename... Pairs>
            struct parse_args<tml::list<Var, Val, TAIL...>, tml::list<Pairs...>>
            {
                using _pairs = tml::list<Pairs...,pair<Var,Val>>;
                
                using pairs = typename parse_args<tml::list<TAIL...>, _pairs>::pairs; 
                using body = typename parse_args<tml::list<TAIL...>, _pairs>::body;
            };

            template<typename Pairs, typename Body>
            struct parse_args<tml::list<Body>, Pairs>
            {
                using pairs = Pairs;
                using body = Body;
            };

            using parser = parse_args<tml::list<ARGS...>,tml::empty_list>;

            using result = typename multi_let_currifier_process<typename parser::pairs, typename parser::body>::result;
        };

        template<template<typename,typename>class... Pairs, typename... Vars, typename... Vals, typename Body>
        struct multi_let_currifier<tml::list<Pairs<Vars,Vals>...>, Body>
        {
            using result = typename multi_let_currifier_process<tml::list<Pairs<Vars,Vals>...>, Body>::result;
        };

    }
    
    /*
     * Haskell-like multiple-variable let expression
     * 
     * Binds a  set of values to a set of named variables in the specified expression.
     * This metafunction 
     *  - NAME: The name (variable) to bind the value with.
     *  - VALUE: The value to be binded.
     *  - EXPRESSION: The expression where the substitution will be applied.
     * 
     * The result of this metafunction is a copy of the passed expression where 
     * the ocurences of the named variable have been substituted with the value.
     */
    template<typename... ARGS>
    using multi_let = typename impl::multi_let_currifier<tml::list<ARGS...>>::result;
}

#endif  /* LET_HPP */