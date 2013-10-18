/* 
 * File:   sqrt.hpp
 * Author: manu343726
 *
 * Created on 26 de septiembre de 2013, 17:53
 */

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
    using default_sqrt_newton_iterations = mpl::uinteger<10>;
#else
    using default_sqrt_newton_iterations = mpl::uinteger<TURBO_CUSTOM_SQRT_DEFAULT_ITERATIONS>;
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
            using result = decltype( PREVIOUS() - ((mpl::square<PREVIOUS>() - N() ) / (PREVIOUS() + PREVIOUS())) );
            using abort  = mpl::equal<PREVIOUS , result>; //When the function converges, the loop is aborted
        };
        
        using begin = mpl::make_uinteger_forward_iterator<1>;
        using end   = mpl::make_uinteger_forward_iterator<ITERATIONS::value>;
        using guess = mpl::one<N>;
        
        using result = mpl::for_loop<begin,end,guess,newton_iter_kernel>;
    };
    
    template<typename N , typename ITERATIONS = math::default_sqrt_newton_iterations>
    using sqrt = typename sqrt_t<N,ITERATIONS>::result;
}

#endif	/* SQRT_HPP */

