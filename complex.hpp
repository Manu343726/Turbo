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
    struct length_t<math::complex<REAL,IMAGINARY>> : public tb::function<math::sqrt<decltype( REAL()*REAL() + IMAGINARY()*IMAGINARY() )>> {};
    
    template<typename REAL , typename IMAGINARY>
    struct square_length_t<math::complex<REAL,IMAGINARY>> : public tb::function<decltype( REAL()*REAL() + IMAGINARY()*IMAGINARY() )> {};
}

namespace tb
{
    template<typename REAL , typename IMAGINARY>
    struct to_string_t<math::complex<REAL,IMAGINARY>>
    {
        operator std::string()
        {
            std::ostringstream os;
            
            os << "(" << tb::to_string<REAL>() << "," << tb::to_string<IMAGINARY>() << "i)";
            
            return os.str();
        }
    };
    
    
    template<typename R1 , typename I1 , typename R2 , typename I2>
    struct add_t<math::complex<R1,I1> , math::complex<R2,I2>> : public tb::function<math::complex<tb::add<R1,R2>,tb::add<I1,I2>>> {};
    
    template<typename R1 , typename I1 , typename R2 , typename I2>
    struct sub_t<math::complex<R1,I1> , math::complex<R2,I2>> : public tb::function<math::complex<tb::sub<R1,R2>,tb::sub<I1,I2>>> {};
    
    template<typename R1 , typename I1 , typename R2 , typename I2>
    struct mul_t<math::complex<R1,I1> , math::complex<R2,I2>> : public tb::function<math::complex<decltype( R1()*R2() - I1()*I2() ) , decltype( I1()*R2() + R1()*I2() )>> {};
    
    template<typename A , typename B , typename C , typename D>
    struct div_t<math::complex<A,B> , math::complex<C,D>> : public tb::function<math::complex<decltype( ( A()*C() + B()*D() ) / ( C()*C() + D()*D() ) ) , decltype( ( B()*C() - A()*D() ) / ( C()*C() + D()*D() ) )>> {};
}

#endif	/* COMPLEX_HPP */

