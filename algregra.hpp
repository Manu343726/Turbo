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

#ifndef ALGREGRA_HPP
#define	ALGREGRA_HPP

#include "function_alias_decl.hpp"


/*
 * This header declares the set of basic algebra operations, their implementation templates
 * in tml::impl namespace, and the user-side template_aliases.
 * 
 * Also this header defines a set operators overloading which implement that algebra operations.
 * That operators implement expression templates which compute the desired algebra operation.
 * The result could be extracted via a decltype() aplication, or the macro ALGEVAL(), which is just
 * a shorthand for decltype.
 */

namespace tml
{
    namespace impl
    {
        /* Basic algebraic operations */
        
        /*
         * Performs the addition of two values
         */
        template<typename LHS , typename RHS>
        struct add;
        
        /*
         * Performs the substraction of two values
         */
        template<typename LHS , typename RHS>
        struct sub;
        
        /*
         * Performs the multiplication of two values
         */
        template<typename LHS , typename RHS>
        struct mul;
        
        /*
         * Performs the division of two values
         */
        template<typename LHS , typename RHS>
        struct div;
    }
    
    TURBO_DEFINE_FUNCTION_TALIAS( add );
    TURBO_DEFINE_FUNCTION_TALIAS( sub );
    TURBO_DEFINE_FUNCTION_TALIAS( mul );
    TURBO_DEFINE_FUNCTION_TALIAS( div );
}

#endif	/* ALGREGRA_HPP */

