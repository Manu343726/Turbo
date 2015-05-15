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

#ifndef MATH_HPP
#define	MATH_HPP

#include "algorithm.hpp"
#include "algebra.hpp"
#include "integral.hpp"
#include "lazy.hpp"
#include "type_traits.hpp"
#include "control_structures.hpp"
#include "eval.hpp"

/*
 * This header defined some matematical metafunctions
 */

namespace tml
{
    /*
     * The tml::pow metafunction returns the value of BASE raised to the value of EXP.
     * Valid tml::sign , tml::mul , and tml::div specializations should exist.
     * 
     * Note that this operation is fully generic, relying only on the different algebraic operations
     * provided to operate on the parameters. That has some consequences, for example note that
     * negative exponent with integral values has no sense, but its perfectly computed and returns 
     * zero always.
     */
    template<typename BASE , typename EXP>
    struct pow;
    
    /*
     * Computes the square root of a given number N
     */
    template<typename N>
    struct sqrt
    {
        /*
         * Square Root via Newton Raphson aproximation:
         * 
         * sqrt(a) = root(f(x)) where:
         * 
         * f(x) = X^2 - a
         * f'(x) = 2X
         * 
         *                f(Xn)             Xn * Xn - a 
         * Xn+1 = Xn - ___________ = Xn - _______________
         *                f'(Xn)                2Xn 
         */
        
        template<typename Xn , typename COUNTER /* unused */>
        struct iteration
        {
            using type = tml::eval<tml::sub<Xn,tml::div<tml::sub<tml::mul<Xn,Xn>,N>,tml::add<Xn,Xn>>>>;
        };

        
        using type = tml::apply_for<tml::lazy<iteration> , N , tml::size_t<0> , tml::size_t<5>>;
        
        TURBO_ENSURE_ALGEBRAIC_EVAL(type);
    };
}

#endif	/* MATH_HPP */

