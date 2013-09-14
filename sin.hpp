/* 
 * File:   sin.hpp
 * Author: manu343726
 *
 * Created on 14 de septiembre de 2013, 16:30
 */

#ifndef SIN_HPP
#define	SIN_HPP

#include "fixed_point.hpp"
#include "basic_types.hpp"
#include "control_flow.hpp"
#include "operators.hpp"
#include "numeric_iterators.hpp"

namespace math
{
    template<typename T , typename TERMS_COUNT = mpl::uinteger<4>>
    struct sin_t;
    
    template<typename T , typename TERMS_COUNT = mpl::uinteger<4>>
    using sin = typename sin_t<T,TERMS_COUNT>::result;
    
    /*
     * sin() function implementation through Taylor series (Check http://www10.informatik.uni-erlangen.de/~pflaum/pflaum/ProSeminar/meta-art.html)
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
    
    template<mpl::fpbits BITS , mpl::fbcount PRECISION , unsigned int TERMS_COUNT>
    struct sin_t<mpl::fixed_point<BITS,PRECISION>,mpl::uinteger<TERMS_COUNT>>
    {
    private:
        using x = mpl::fixed_point<BITS,PRECISION>;
        
        using begin = mpl::make_integer_backward_iterator<TERMS_COUNT-1>;
        using end   = mpl::make_integer_backward_iterator<-1>;
        
        using one   = mpl::decimal<1,0,PRECISION>;
        using two   = mpl::decimal<2,0,PRECISION>;
        using three = mpl::decimal<3,0,PRECISION>;
        
        template<typename K , typename VAL>
        struct kernel : public mpl::function<decltype( one() - ( x() * x() )/(two() * K() + two())/(two()*K()+three())*VAL() )> {};
        
    public:
        using result = decltype( x() * mpl::for_loop<begin , end , one , kernel>() );
    };
}

#endif	/* SIN_HPP */

