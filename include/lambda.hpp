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

#ifndef LAMBDA_HPP
#define	LAMBDA_HPP

#include "manu343726/turbo_core/eval.hpp"
#include "manu343726/turbo_core/let_expressions.hpp"
#include "manu343726/turbo_core/list_algorithms.hpp"

namespace tml
{ 
    namespace impl
    {
        namespace 
        {
            template<typename T, typename U>
            struct pair {};
        }

        template<typename LambdaArgs, typename CallArgs, typename Pairs>
        struct parse_lambda;

        template<typename Variable, typename Value, typename... ArgsTail, typename... CallTail,
                 typename... Variables, typename... Values>
        struct parse_lambda<tml::list<Variable, ArgsTail...>, tml::list<Value, CallTail...>,
                            tml::list<pair<Variables,Values>...>
                           >
        {
            using next = parse_lambda<tml::list<ArgsTail...>, tml::list<CallTail...>,
                                      tml::list<pair<Variables,Values>...,pair<Variable,Value>>
                                     >;

            using body = typename next::body;
            using pairs = typename next::pairs;
        };

        template<typename Body, typename Pairs>
        struct parse_lambda<tml::list<Body>, tml::empty_list, Pairs>
        {
            using pairs = Pairs;
            using body = Body;
        };
    }
    
    /*
     * Represents a multiple-variable lambda expression.
     * 
     * A lambda expression is just a template that holds a set of variables and a functional
     * expression (The lambda body) where the variable is used. 
     * The lambda expression is designed to act as a functional expression to be evaluated lazily,
     * when the values of the parameters are specified. Thats why the 'result'  member of function entities
     * is parametrized.
     * 
     * Because the 'result' member doesn't work like in other function entities (In other function entities returns
     * the result, in a lambda computes the result), a lambda cannot be considered a classic function entity.
     * The implementation trait 'tml::impl::is_function' and the 'tml::eval' implementation are overrided to
     * cover this special behaviour of lambda expressions. 
     */
    template<typename... LambdaArgs>
    struct lambda
    {
        template<typename... CallArgs>
        struct apply
        {
            using parser = tml::impl::parse_lambda<tml::list<LambdaArgs...>, tml::list<CallArgs...>, tml::empty_list>;
            using pairs = typename parser::pairs;
            using body = typename parser::body;

            using result = tml::eval<typename tml::impl::multi_let_currifier<pairs, body>::result>;
        };
    };
}

#endif	/* LAMBDA_HPP */









