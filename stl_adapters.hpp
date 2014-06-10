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

#ifndef STL_ADAPTERS_HPP
#define	STL_ADAPTERS_HPP

#include "basic_types.hpp"
#include "lazy.hpp"


/*
 * The Standard library uses a member type '::type' on metafunctions like type 
 * traits to return the result, whereas Turbo uses '::result'.
 * 
 * The utilities defined bellow help to use STL metafunctions in Turbo features.
 */

namespace tml
{
    namespace stl
    {
        /*
         * Adapts a Standard Library metafunction to Turbo
         */
        template<typename F>
        using function = tml::function<typename F::type>;
        
        /*
         * Adapts a Standard Library metafunction to Turbo lazily
         */
        template<template<typename...> class F>
        struct lazy
        {
            template<typename... ARGS>
            using instance = typename F<ARGS...>::type;
        };
        
        /*
         * Evaluates the result of a STL metafunction
         */
        template<typename F>
        using eval = typename F::type;
        
        /*
         * Evaluates the result of a STL metafunction on a let expression context
         */
        template<typename F , typename... ARGS>
        using deval = tml::deval<tml::stl::function<F>,ARGS...>;
    }
    
    /*
     * Evaluation for lazy adapters
     */
    
    template<template<typename...> class F>
    struct overrides_eval<tml::stl::lazy<F>> : public tml::true_type
    {};
    
    namespace impl
    {
        template<template<typename...> class F , typename ARG , typename... ARGS>
        struct eval<tml::stl::lazy<F>,tml::list<ARG,ARGS...>> : 
            public tml::function<tml::eval<tml::lazy_instance<tml::stl::lazy<F>,ARG,ARGS...>>>
        {};
    }
}

#endif	/* STL_ADAPTERS_HPP */

