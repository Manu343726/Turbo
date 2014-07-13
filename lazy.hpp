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

#ifndef LAZY_HPP
#define	LAZY_HPP

#include "eval.hpp"

namespace tml
{
    /*
     * Wraps a template to provide lazy template instantation
     * 
     * 
     * There are situations where a template will be evaluated/instantiated later, but the 
     * use of placeholders is not possible due to the way they are used inside the template. For example:
     * 
     *     template<typename V>
     *     using increment = std::integral_constant<int,V::value + 1>;
     * 
     *     using result = tml::map<increment<_1>,tml::integer_list<1,2,3,4,5>>; //ERROR: tml::placeholders::_1 has no member "value"
     * 
     * The template tml::lazy is dessigned to wrap a template in a type (Instead of a template parameter), and instantiate
     * it later with a set of parameters. 
     * This allows the user to pass templates through metafunctions without instantiating them, making the use of placeholders not
     * necessary. Also lazy template instantation could be used to leave complex template instantations (Instantations which could take a long 
     * time/effort) until the point of its ussage.
     * 
     * Also tml::lazy allows to specify the instantation parameter at the declaration point, but note that the template will be instanced only
     * if required explicitly by the user using tml::lazy_instance. For example:
     * 
     *     using lazy = tml::lazy<std::vector,int>; //Here std::vector<int> is not instanced at all.
     *     
     *     using vector = tml::lazy_instance<lazy>; //Here we explicitly instance the wrapped template, std::vector<int> in this example.
     * 
     * This could be usefull to store potentially ill-formed template instantations. For example:
     * 
     *     template<typename T>
     *     struct foo;
     * 
     *     template<>
     *     struct foo<char>{};
     * 
     *     using ok = tml::lazy_instance<tml::conditional<tml::true_type,
     *                                                    tml::lazy<foo,char>,
     *                                                    tml::lazy<foo,bool>
     *                                                   >
     *                                  >;
     * 
     * In the above example, the ill-formed 'foo<bool>' will never be instantiated, because the conditional selects the first branch.
     *                                                    
     */
    template<template<typename...> class F , typename... ARGS>
    struct lazy
    {}; 
    
/*
 * Shorthand
 */
#define L(x) tml::lazy<x>
    
    /*
     * The metafunction lazy_instance takes a wrapped template L and a set of template parameters,
     * and returns the corresponding template instance.
     * 
     * This metafunction is specialized to take care of the two usage cases of tml::lazy:
     *  - Template parameters specified at instantation-point: The template is instanced 
     *    with the parameters passed to the 'lazy_instance' metafunction.
     * 
     *  - Template parameters specified at declaration point: The parameters passed to
     *    'lazy_instance' are ignored, the specified at declaration-point are used instead.
     * 
     * Also the default template works as an identity function, returning the parameter untouched
     * if its not a tml::lazy instance. This behaviour helps in some contexts.
     */
    namespace impl
    {
        /*
         * A lazy instance of an instanced template is the instance itself.
         * 
         *     tml::lazy_instance<int> returns int
         */
        template<typename L , typename... ARGS>
        struct lazy_instance : public tml::function<L>
        {};
       
        /*
         * A lazy instance of a tml::lazy instance is an instance of the wrapped templates where
         * the binded parameters come first, and then the passed parameters:
         * 
         *     tml::lazy_instance<tml::lazy<foo>,int>     returns foo<int>
         *     tml::lazy_instance<tml::lazy<foo,int>>     returns foo<int>
         *     tml::lazy_instance<tml::lazy<foo,int>,int> returns foo<int,int>
         */
        template<template<typename...> class F , typename... ARGS , typename... IARGS>
        struct lazy_instance<tml::lazy<F,ARGS...>,IARGS...> : public tml::function<F<ARGS...,IARGS...>>
        {};
    }
    
    /*
     * The lazy_instance evaluator is provided as a functor too, to be usable on expressions.
     */
    namespace func
    {
        template<typename L , typename... ARGS>
        using lazy_instance = tml::impl::lazy_instance<L,ARGS...>;
    }
    
    
    /*
     * Instantiates a template wrapped by L using the specified template argumments.
     */
    template<typename L , typename... ARGS>
    using lazy_instance = typename tml::impl::lazy_instance<L,ARGS...>::result;
    
    
    
    /*
     * tml::eval is overrided to take care of wrapped templates. A wrapped template should be evaluated with a set of parameters.
     * That parameters are used to instantiate the template and evaluate the resulting instance later.
     */
    
    template<template<typename...> class F , typename... ARGS>
    struct overrides_eval<tml::lazy<F,ARGS...>> : public tml::true_type
    {};
    
    namespace impl
    {
        template<template<typename...> class F , typename... B_ARGS , typename... ARGS>
        struct eval<tml::lazy<F,B_ARGS...>,tml::list<ARGS...>> : 
            public tml::function<tml::eval<tml::lazy_instance<tml::lazy<F,B_ARGS...>,ARGS...>>>
        {};
    }
}

#endif	/* LAZY_HPP */

