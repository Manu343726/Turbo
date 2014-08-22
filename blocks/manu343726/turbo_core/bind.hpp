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

#ifndef BIND_HPP
#define	BIND_HPP

#include "manu343726/turbo_core/algorithm.hpp"
#include "manu343726/turbo_core/placeholders.hpp"
#include "manu343726/turbo_core/curry.hpp"
#include "manu343726/turbo_core/placeholder_lists.hpp"

using namespace tml::placeholders;

namespace tml
{
    
    
    
    namespace impl
    {
        /*
         * Binds some parameters to a function building a function entity which could be called passing the rest
         * of parameters to evaluate the function.
         * It mimics the behaviour of std::bind(), the placeholders behaviour included.
         */
        template<typename F , typename... ARGS>
        struct bind
        {};
    
        template<typename B , typename... CALL_ARGS>
        struct bind_call;
        
        template<typename F , typename... ARGS , typename... CARGS>
        struct bind_call<tml::impl::bind<F,ARGS...>,CARGS...>
        {
            /*
             * Binded function argumments are transformed to their values:
             * 
             *  - If the parameter is not a placeholder, the corresponding function call argumment is the 
             *    parameter itself (Main specialization of 'transformation').
             *  - If the parameter is a placeholder, the corresponding function call argumment is the bind evaluation (call) parameter 
             *    represented with that placeholder, that is, the argumment with index N-1, where the placeholder is _N
             *    (Partial specialization of 'transformation').  
             *    Because tml::lists::get has O(n) complexity, the complexity of evaluating a tml::bind call is O(n^2) in the worst case.
             *    In practice is not, because tml::transform for lists is O(1), so the complexity is O(n) (See the documentation at "algorithm.hpp").
             *    Of course remember that template instantation depth is what that complexity measures.
             */
            template<typename T>
            struct transformation : public tml::function<T>
            {};
            
            template<std::size_t INDEX>
            struct transformation<tml::placeholder<INDEX>> : 
                public tml::lists::func::get<tml::list<CARGS...>,tml::size_t<INDEX>>
            {};
            
            using call_args = tml::transform<tml::lazy<transformation>,
                                             tml::list<ARGS...>
                                            >;
            
            template<typename... RARGS>
            struct apply : public tml::function<tml::eval<F,RARGS...>>
            {};
            
            /*
             * The result is the result of the call
             */
            using result = tml::eval<tml::uncurry<apply>,call_args>;
        };
    }
    
    template<template<typename...> class F , typename... ARGS>
    using bind = tml::impl::bind<tml::lazy<F>,ARGS...>;
    
    /*
     * Like in any other Turbo function entity, the way to evaluate tml::bind is using tml::eval.
     * tml::bind overrides eval for that purpose, taking the evaluation parameters and transforming them
     * to complete the function call with the binded parameters.
     */
    template<template<typename...> class F , typename... ARGS>
    struct overrides_eval<tml::bind<F,ARGS...>> : public tml::true_type
    {};
    
    namespace impl
    {
        template<template<typename...> class F , typename... ARGS , typename... CALL_ARGS>
        struct eval<tml::bind<F,ARGS...>,tml::list<CALL_ARGS...>> : public tml::impl::bind_call<tml::bind<F,ARGS...>,CALL_ARGS...>
        {};
    }
    
    namespace impl
    {
        template<typename F , typename... ARGS>
        struct partial_eval;
        
        template<template<typename...> class F , typename... F_ARGS , typename... ARGS>
        struct partial_eval<F<F_ARGS...>,ARGS...>
        {
            using result = tml::bind<F,ARGS...,tml::placeholders_range<sizeof...(ARGS),sizeof...(F_ARGS)>>;
        };
    }
    
    /*
     * Given a parametrized expression F of m argumments, evaluates partially the expression with n argumments,
     * returning a parametrized expression of m-n argumments. 
     */
    template<typename F , typename... ARGS>
    using peval = typename tml::impl::partial_eval<F,ARGS...>::result;
}

#endif	/* BIND_HPP */

