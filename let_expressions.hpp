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
#define	LET_HPP

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
         * General case of the high-level phase: Just parameter forwarding to the 
         * second phase.
         */
        template<typename NAME , typename VALUE , typename EXPRESSION>
        struct let_impl_high;
        
    }
}

#endif	/* LET_HPP */

