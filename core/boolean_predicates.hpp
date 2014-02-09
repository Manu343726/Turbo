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

#ifndef BOOLEAN_PREDICATES_HPP
#define	BOOLEAN_PREDICATES_HPP

#include "basic_types.hpp"
#include "function.hpp"
#include "arithmetic_functions.hpp"


namespace tml
{
    template<typename T = tml::null>
    using true_function = tml::function<tml::true_type>;
    
    template<typename T = tml::null>
    using false_function = tml::function<tml::false_type>;
    
    template<typename T = tml::null>
    using true_predicate = tml::true_type;
    
    template<typename T = tml::null>
    using false_predicate = tml::false_type;
}

#endif	/* BOOLEAN_PREDICATES_HPP */

