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

#include "functional.hpp"

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
     */
    template<template<typename...> class F>
    struct lazy 
    {
        template<typename... ARGS>
        using instance = F<ARGS...>;
    };
    
    /*
     * Instantiates a template wrapped by L using the specified template argumments.
     */
    template<typename L , typename... ARGS>
    using lazy_instance = typename L::template instance<ARGS...>;
    
    
    
    /*
     * tml::eval is overrided to take care of wrapped templates. A wrapped template should be evaluated with a set of parameters.
     * That parameters are used to instantiate the template and evaluate the resulting instance later.
     */
    namespace impl
    {
        template<template<typename...> class F , typename ARG , typename... ARGS>
        struct evaluate_impl<tml::lazy<F>,ARG,ARGS...> : 
        public tml::function<tml::eval<tml::lazy_instance<tml::lazy<F>,ARG,ARGS...>>>
        {};
    }
}

#endif	/* LAZY_HPP */

