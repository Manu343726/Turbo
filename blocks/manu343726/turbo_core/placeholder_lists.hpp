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

#ifndef PLACEHOLDER_LISTS_HPP
#define	PLACEHOLDER_LISTS_HPP

#include "manu343726/turbo_core/list.hpp"
#include "manu343726/turbo_core/placeholders.hpp"
#include "manu343726/turbo_core/high_order_functions.hpp"
#include "manu343726/turbo_core/lazy.hpp"
#include "manu343726/turbo_core/integral_iterators.hpp"


/*
 * Here we define a set of utilities to create and manipulate lists and ranges of placeholders.
 */

namespace tml
{
    /*
     * Returns a tml::list containing the placeholders _i1 , _i2 , ... , _in; where
     * i1 , i2 , ... , in are the passed indices.
     */
    template<std::size_t... INDICES>
    using placeholders_list = tml::list<tml::placeholder<INDICES>...>;
    
    /*
     * Returns a list containing the placeholders on the interval [BEGIN,END), that is
     * the placeholders _b , _b+1 , _b+2 , ... , _e-2 , _e-1.
     */
    template<std::size_t BEGIN , std::size_t END>
    using placeholders_range = tml::transform<tml::lazy<tml::function>,
                                              tml::integral_forward_iterators::make_size_t<BEGIN>,
                                              tml::integral_forward_iterators::make_size_t<END>
                                             >;
}

#endif	/* PLACEHOLDER_LISTS_HPP */

