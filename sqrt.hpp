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

#ifndef SQRT_HPP
#define	SQRT_HPP

#include "fixed_point.hpp"
#include "value_t.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "for_loops.hpp"
#include "numeric_iterators.hpp"

/*
 * Square root implementation through Newton's method:
 * 
 * sqrt(N)
 * 
 * The function to be "rooted" (Function which we need a root) is:
 * f(x) = x² - N
 * 
 * Its first derivative is:
 * f'(x) = 2x
 * 
 * So the recursive equation is:
 *             X²n - N
 * Xn+1 = Xn + -------
 *               2Xn
 */

namespace math
{
    
#ifndef TURBO_CUSTOM_SQRT_DEFAULT_ITERATIONS
    using default_sqrt_newton_iterations = tb::uinteger<10>;
#else
    using default_sqrt_newton_iterations = tb::uinteger<TURBO_CUSTOM_SQRT_DEFAULT_ITERATIONS>;
#endif
    
    template<typename N , typename ITERATIONS>
    struct sqrt_t
    {       
        /*
         * For loop kernel: CURRENT (The value of the loop counter) is not used. The previous value holds Xn.
         */
        template<typename CURRENT , typename PREVIOUS>                                //2*Xn was rewritten as Xn + Xn due to a lack of the library...
        struct newton_iter_kernel 
        {
            using result = decltype( PREVIOUS() - ((tb::square<PREVIOUS>() - N() ) / (PREVIOUS() + PREVIOUS())) );
            using abort  = tb::equal<PREVIOUS , result>; //When the function converges, the loop is aborted
        };
        
        using begin = tb::make_uinteger_forward_iterator<1>;
        using end   = tb::make_uinteger_forward_iterator<ITERATIONS::value>;
        using guess = tb::one<N>;
        
        using result = tb::for_loop<begin,end,guess,newton_iter_kernel>;
    };
    
    template<typename N , typename ITERATIONS = math::default_sqrt_newton_iterations>
    using sqrt = typename sqrt_t<N,ITERATIONS>::result;
}

#endif	/* SQRT_HPP */

