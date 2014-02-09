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

#ifndef SIN_HPP
#define	SIN_HPP

#include "core.hpp"
#include "fixed_point.hpp"
#include "for_loops.hpp"
#include "numeric_iterators.hpp"

namespace math
{
    template<tml::fixed_point_precision PRECISION>
    struct pi_t : public tml::function<tml::decimal<3141592654,-9,PRECISION>> {};
    
    template<>
    struct pi_t<0> : public tml::function<tml::decimal<3,0,0>> {};
    
    template<>
    struct pi_t<1> : public tml::function<tml::decimal<31,-1,1>> {};
    
    template<>
    struct pi_t<2> : public tml::function<tml::decimal<314,-2,2>> {};
    
    template<>
    struct pi_t<3> : public tml::function<tml::decimal<3141,-3,3>> {};
    
    template<>
    struct pi_t<4> : public tml::function<tml::decimal<31416,-4,4>> {};
    
    template<>
    struct pi_t<5> : public tml::function<tml::decimal<314159,-5,5>> {};
    
    template<>
    struct pi_t<6> : public tml::function<tml::decimal<3141592,-6,6>> {};
    
    template<>
    struct pi_t<7> : public tml::function<tml::decimal<31415926,-7,7>> {};
    
    template<>
    struct pi_t<8> : public tml::function<tml::decimal<314159265,-8,8>> {};
    
    template<tml::fixed_point_precision PRECISION = tml::DEFAULT_FRACTIONAL_PRECISION>
    using concrete_pi = typename math::pi_t<PRECISION>::result;
    
    using pi = concrete_pi<tml::DEFAULT_FRACTIONAL_PRECISION>;
    
    
    
#ifndef TURBO_CUSTOM_TRIGONOMETRY_DEFAULT_TERMS
    using default_trigonometry_approximation_terms = tml::uinteger<10>;
#else
    using default_trigonometry_approximation_terms = tml::uinteger<TURBO_CUSTOM_TRIGONOMETRY_DEFAULT_TERMS>;
#endif
    
    template<typename T , typename TERMS_COUNT>
    struct sin_t;
    
    template<typename T , typename TERMS_COUNT = math::default_trigonometry_approximation_terms>
    using sin = typename sin_t<T,TERMS_COUNT>::result;
    
    /*
     * sin() function implementation through Taylor (McLaurin) series.  
     * 
     * NOTE: The function explodes the recursive expansion of the terms of the series:
     *                                        n
     *       The original series are: sin x = Σ {(-1)^k x^(2k+1)/(2k+1)!} + R
     *                                       k=0
     * 
     *       taking the expansion up to the 5th term gives (in a more prosaic notation):
     * 
     *       sin x = x - x^3/3! + x^5/5! - x^7/7! + x^9/9! = x{1 - x^2/3! + x^4/5! - x^6/7! + x^8/9!}
     * 
     *       The part in parentheses can be rewritten in turn as:
     * 
     *       1 - x^2/(2·3) {1 - x^2/(4·5) + x^4/(4·5·6·7) - x^6/(4·5·6·7·8·9)}
     * 
     *       again the part in braces can be rewritten as:
     * 
     *       1 - x^2/(4·5) {1 - x^2/(6·7) + x^4/(6·7·8·9)}
     * 
     *       and so on. The function is just the implementation of this recursive rewriting.
     * 
     * The C equivalent code is:
     * 
     * // Calculate sin(x) using j terms
     * float sine(float x, int j)
     * {
     *     float val = 1;
     *
     *     for (int k = j - 1; k >= 0; --k)
     *         val = 1 - x*x/(2*k+2)/(2*k+3)*val;
     *
     *     return x * val;
     * }
     */
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION , unsigned int TERMS_COUNT>
    struct sin_t<tml::fixed_point<BITS,PRECISION>,tml::uinteger<TERMS_COUNT>>
    {
    private:
        using x = tml::fixed_point<BITS,PRECISION>;
        
        using begin = tml::make_integer_backward_iterator<TERMS_COUNT-1>;
        using end   = tml::make_integer_backward_iterator<-1>;
        
        using one   = tml::decimal<1,0,PRECISION>;
        using two   = tml::decimal<2,0,PRECISION>;
        using three = tml::decimal<3,0,PRECISION>;
        
        template<typename K , typename VAL>
        struct kernel
        {
            using result = decltype( one() - ( x() * x() )/(two() * K() + two())/(two()*K()+three())*VAL() );
            using abort = tml::equal<result,VAL>; //When the value converges, the loop is aborted
        };
        
    public:
        using result = decltype( x() * tml::for_loop<begin , end , one , kernel>() );
    };
    
    
    
    template<typename T , typename TERMS_COUNT>
    struct cos_t;
    
    template<typename T , typename TERMS_COUNT = math::default_trigonometry_approximation_terms>
    using cos = typename cos_t<T,TERMS_COUNT>::result;
    
    /*
     * cos() function implementation through Taylor (McLaurin) series.  
     * 
     * NOTE: The function explodes the recursive expansion of the terms of the series,
     *       exactly as in the sine implementation.
     * 
     * The C equivalent code is:
     * 
     * // Calculate cos(x) using j terms
     * float cosine(float x, int j)
     * {
     *     float val = 1;
     *
     *     for (int k = j - 1; k >= 0; --k)
     *         val = 1 - x*x/(2*k+2)/(2*k+1)*val;
     *
     *     return val;
     * }
     */
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION , unsigned int TERMS_COUNT>
    struct cos_t<tml::fixed_point<BITS,PRECISION>,tml::uinteger<TERMS_COUNT>>
    {
    private:
        using x = tml::fixed_point<BITS,PRECISION>;
        
        using begin = tml::make_integer_backward_iterator<TERMS_COUNT-1>;
        using end   = tml::make_integer_backward_iterator<-1>;
        
        using one   = tml::decimal<1,0,PRECISION>;
        using two   = tml::decimal<2,0,PRECISION>;
        
        template<typename K , typename VAL>
        struct kernel
        {
            using result = decltype( one() - ( x() * x() )/(two() * K() + two())/(two()*K()+one())*VAL() );
            using abort = tml::equal<result,VAL>; //When the value converges, the loop is aborted
        };
    public:
        using result = tml::for_loop<begin , end , one , kernel>;
    };
    
    
    template<typename T , typename TERMS_COUNT = tml::uinteger<10>>
    using tan   = tml::div<sin<T,TERMS_COUNT>,cos<T,TERMS_COUNT>>;
    
    template<typename T , typename TERMS_COUNT = tml::uinteger<10>>
    using cotan = tml::div<cos<T,TERMS_COUNT>,sin<T,TERMS_COUNT>>;
}

#endif	/* SIN_HPP */

