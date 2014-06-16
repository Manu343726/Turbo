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

#ifndef CURRY_HPP
#define	CURRY_HPP

#include "function.hpp"
#include "list.hpp"

namespace tml
{
    /*
     * Given a function F with one tuple argumment (tml::list<ARGS...>),
     * tml::curry<F> is a equivalent function which expects n argumments instead
     * of a tuple (tml::list) of argumments.
     */
    template<template<typename> class F>
    struct curry
    {};

    /*
     * Given a function F with n argumments (A variadic-pack ARGS...),
     * tml::uncurry<F> is a equivalent function which expects one tuple argumment
     * instead of n argumments.
     */
    template<template<typename...> class F>
    struct uncurry
    {};

    template<template<typename> class F>
    struct overrides_eval<tml::curry<F>> : public tml::true_type
    {};
    template<template<typename...> class F>
    struct overrides_eval<tml::uncurry<F>> : public tml::true_type
    {};
    
    namespace impl
    {
        template<template<typename> class F , typename... CARGS>
        struct eval<tml::curry<F> , tml::list<CARGS...>> : 
            public tml::function<tml::eval<F<tml::list<CARGS...>>>>
        {};
        template<template<typename...> class F , typename... CARGS>
        struct eval<tml::uncurry<F> , tml::list<tml::list<CARGS...>>> : 
            public tml::function<tml::eval<F<CARGS...>>>
        {};
    }
    
}

#endif	/* CURRY_HPP */

