/* 
 * File:   complex.hpp
 * Author: manu343726
 *
 * Created on 30 de septiembre de 2013, 0:48
 */

#ifndef COMPLEX_HPP
#define	COMPLEX_HPP

#include "operators.hpp"
#include "expressions.hpp"
#include "basic_types.hpp"
#include "fixed_point.hpp"
#include "sqrt.hpp"
#include "to_string.hpp"
#include "color.hpp"

#include <string>
#include <sstream>

namespace math
{
    template<typename REAL , typename IMAGINARY>
    struct complex
    {
        using real      = REAL;
        using imaginary = IMAGINARY;
    };
    
    template<typename REAL , typename IMAGINARY>
    struct length_t<math::complex<REAL,IMAGINARY>> : public mpl::function<math::sqrt<decltype( REAL()*REAL() + IMAGINARY()*IMAGINARY() )>> {};
    
    template<typename REAL , typename IMAGINARY>
    struct square_length_t<math::complex<REAL,IMAGINARY>> : public mpl::function<decltype( REAL()*REAL() + IMAGINARY()*IMAGINARY() )> {};
}

namespace mpl
{
    template<typename REAL , typename IMAGINARY>
    struct to_string_t<math::complex<REAL,IMAGINARY>>
    {
        operator std::string()
        {
            std::ostringstream os;
            
            os << "(" << mpl::to_string<REAL>() << "," << mpl::to_string<IMAGINARY>() << "i)";
            
            return os.str();
        }
    };
    
    
    template<typename R1 , typename I1 , typename R2 , typename I2>
    struct add_t<math::complex<R1,I1> , math::complex<R2,I2>> : public mpl::function<math::complex<mpl::add<R1,R2>,mpl::add<I1,I2>>> {};
    
    template<typename R1 , typename I1 , typename R2 , typename I2>
    struct sub_t<math::complex<R1,I1> , math::complex<R2,I2>> : public mpl::function<math::complex<mpl::sub<R1,R2>,mpl::sub<I1,I2>>> {};
    
    template<typename R1 , typename I1 , typename R2 , typename I2>
    struct mul_t<math::complex<R1,I1> , math::complex<R2,I2>> : public mpl::function<math::complex<decltype( R1()*R2() - I1()*I2() ) , decltype( I1()*R2() + R1()*I2() )>> {};
    
    template<typename A , typename B , typename C , typename D>
    struct div_t<math::complex<A,B> , math::complex<C,D>> : public mpl::function<math::complex<decltype( ( A()*C() + B()*D() ) / ( C()*C() + D()*D() ) ) , decltype( ( B()*C() - A()*D() ) / ( C()*C() + D()*D() ) )>> {};
}

#endif	/* COMPLEX_HPP */

