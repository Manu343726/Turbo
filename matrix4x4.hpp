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

#ifndef MATRIX4X4_HPP
#define	MATRIX4X4_HPP

#include "operators.hpp"
#include "to_string.hpp"
#include "trigonometry.hpp"
#include "transformations.hpp"
#include "vector.hpp"

#include <sstream>
#include <string>


namespace math
{
        template<typename M11 , typename M12 , typename M13 , typename M14 ,
                 typename M21 , typename M22 , typename M23 , typename M24 ,
                 typename M31 , typename M32 , typename M33 , typename M34 ,
                 typename M41 , typename M42 , typename M43 , typename M44>
        struct matrix4x4
        {
            using m11 = M11; using m12 = M12; using m13 = M13; using m14 = M14;
            using m21 = M21; using m22 = M22; using m23 = M23; using m24 = M24;
            using m31 = M31; using m32 = M32; using m33 = M33; using m34 = M34;
            using m41 = M41; using m42 = M42; using m43 = M43; using m44 = M44; 
        };
        
        
        /* unitary matrix */
        
        template<typename T>
        using unity4x4 = math::matrix4x4<tb::one<T>  , tb::zero<T> , tb::zero<T> , tb::zero<T> ,
                                         tb::zero<T> , tb::one<T>  , tb::zero<T> , tb::zero<T> ,
                                         tb::zero<T> , tb::zero<T> , tb::one<T>  , tb::zero<T> ,
                                         tb::zero<T> , tb::zero<T> , tb::zero<T> , tb::one<T>>;
        
        
        /* 2d transformations */
        
        template<typename X , typename Y , typename Z>
        struct translate_t<math::vec3<X,Y,Z>> : public tb::function<math::matrix4x4<tb::one<X>  , tb::zero<X> , tb::zero<X> , X ,
                                                                                     tb::zero<X> , tb::one<X>  , tb::zero<X> , Y ,
                                                                                     tb::zero<X> , tb::zero<X> , tb::one<X>  , Z ,
                                                                                     tb::zero<X> , tb::zero<X> , tb::zero<X> , tb::one<X>>>
        {};
        
        template<typename X , typename Y , typename Z>
        struct scale_t<math::vec3<X,Y,Z>> : public tb::function<math::matrix4x4<      X      , tb::zero<X> , tb::zero<X> , tb::zero<X> ,
                                                                                 tb::zero<X> ,       Y      , tb::zero<X> , tb::zero<X> ,
                                                                                 tb::zero<X> , tb::zero<X> ,       Z      , tb::zero<X> ,
                                                                                 tb::zero<X> , tb::zero<X> , tb::zero<X> , tb::one<X>>>
        {};
        
        /* Rotations (T is the number of terms used by the trigonometric aproximations) */
        
        template<typename ANGLE,typename T>
        struct rotate_t<ANGLE,math::x_axis,T> : public tb::function<math::matrix4x4<tb::one<ANGLE>  ,           tb::zero<ANGLE>        ,  tb::zero<ANGLE>  , tb::zero<ANGLE> , 
                                                                                     tb::zero<ANGLE> ,          math::cos<ANGLE,T>       , math::sin<ANGLE,T> , tb::zero<ANGLE> , 
                                                                                     tb::zero<ANGLE> , tb::opposite<math::sin<ANGLE,T>> , math::cos<ANGLE,T> , tb::zero<ANGLE> ,
                                                                                     tb::zero<ANGLE> ,           tb::zero<ANGLE>        ,  tb::zero<ANGLE>  , tb::one<ANGLE>>>
        {};
        
        template<typename ANGLE,typename T>
        struct rotate_t<ANGLE,math::y_axis,T> : public tb::function<math::matrix4x4<math::cos<ANGLE,T> , tb::zero<ANGLE> , tb::opposite<math::sin<ANGLE,T>> , tb::zero<ANGLE> , 
                                                                                     tb::zero<ANGLE>   , tb::one<ANGLE>  ,           tb::zero<ANGLE>        , tb::zero<ANGLE> , 
                                                                                     math::sin<ANGLE,T> , tb::zero<ANGLE> ,          math::cos<ANGLE,T>       , tb::zero<ANGLE> ,
                                                                                     tb::zero<ANGLE>   , tb::zero<ANGLE> ,           tb::zero<ANGLE>        , tb::one<ANGLE>>>
        {};
        
        template<typename ANGLE,typename T>
        struct rotate_t<ANGLE,math::z_axis,T> : public tb::function<math::matrix4x4<math::cos<ANGLE,T>                , math::sin<ANGLE,T> , tb::zero<ANGLE> , tb::zero<ANGLE> , 
                                                                                     tb::opposite<math::sin<ANGLE,T>> , math::cos<ANGLE,T> , tb::zero<ANGLE> , tb::zero<ANGLE> , 
                                                                                     tb::zero<ANGLE>                  , tb::zero<ANGLE>   , tb::one<ANGLE>  , tb::zero<ANGLE> ,
                                                                                     tb::zero<ANGLE>                  , tb::zero<ANGLE>   , tb::zero<ANGLE> , tb::one<ANGLE>>>
        {};
}

namespace tb
{
        /* to string function specialization */
        
        template<typename M11 , typename M12 , typename M13 , typename M14 ,
                 typename M21 , typename M22 , typename M23 , typename M24 ,
                 typename M31 , typename M32 , typename M33 , typename M34 ,
                 typename M41 , typename M42 , typename M43 , typename M44>
        
        struct to_string_t<math::matrix4x4<M11,M12,M13,M14,
                                           M21,M22,M23,M24,
                                           M31,M32,M33,M34,
                                           M41,M42,M43,M44>>
        {
            operator std::string()
            {
                std::ostringstream os;
                
                os << "|" << tb::to_string<M11>() << "," << tb::to_string<M12>() << "," << tb::to_string<M13>() << "," << tb::to_string<M14>() << "|" << std::endl;
                os << "|" << tb::to_string<M21>() << "," << tb::to_string<M22>() << "," << tb::to_string<M23>() << "," << tb::to_string<M24>() << "|" << std::endl;
                os << "|" << tb::to_string<M31>() << "," << tb::to_string<M32>() << "," << tb::to_string<M33>() << "," << tb::to_string<M34>() << "|" << std::endl;
                os << "|" << tb::to_string<M41>() << "," << tb::to_string<M42>() << "," << tb::to_string<M43>() << "," << tb::to_string<M44>() << "|";
                
                return os.str();
            }
        };
                     
        
        
        /* Arithmetic functions */
        
        
        // matrix + matrix:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 , typename LHS14 ,
                 typename LHS21 , typename LHS22 , typename LHS23 , typename LHS24 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , typename LHS34 ,
                 typename LHS41 , typename LHS42 , typename LHS43 , typename LHS44 ,
                
                 typename RHS11 , typename RHS12 , typename RHS13 , typename RHS14 ,
                 typename RHS21 , typename RHS22 , typename RHS23 , typename RHS24 ,
                 typename RHS31 , typename RHS32 , typename RHS33 , typename RHS34 ,
                 typename RHS41 , typename RHS42 , typename RHS43 , typename RHS44>
        struct add_t<
                     math::matrix4x4<LHS11,LHS12,LHS13,LHS14,
                                     LHS21,LHS22,LHS23,LHS24,
                                     LHS31,LHS32,LHS33,LHS34,
                                     LHS41,LHS42,LHS43,LHS44> , 
                
                     math::matrix4x4<RHS11,RHS12,RHS13,RHS14,
                                     RHS21,RHS22,RHS23,RHS24,
                                     RHS31,RHS32,RHS33,RHS34,
                                     RHS41,RHS42,RHS43,RHS44>
                    > : public tb::function<math::matrix4x4<tb::add<LHS11,RHS11> , tb::add<LHS12,RHS12> , tb::add<LHS13,RHS13> , tb::add<LHS14,RHS14> ,
                                                             tb::add<LHS21,RHS21> , tb::add<LHS22,RHS22> , tb::add<LHS23,RHS23> , tb::add<LHS24,RHS24> ,
                                                             tb::add<LHS31,RHS31> , tb::add<LHS32,RHS32> , tb::add<LHS33,RHS33> , tb::add<LHS34,RHS34> ,
                                                             tb::add<LHS41,RHS41> , tb::add<LHS42,RHS42> , tb::add<LHS43,RHS43> , tb::add<LHS44,RHS44>>>
        {};
        
        
        // matrix - matrix:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 , typename LHS14 ,
                 typename LHS21 , typename LHS22 , typename LHS23 , typename LHS24 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , typename LHS34 ,
                 typename LHS41 , typename LHS42 , typename LHS43 , typename LHS44 ,
                
                 typename RHS11 , typename RHS12 , typename RHS13 , typename RHS14 ,
                 typename RHS21 , typename RHS22 , typename RHS23 , typename RHS24 ,
                 typename RHS31 , typename RHS32 , typename RHS33 , typename RHS34 ,
                 typename RHS41 , typename RHS42 , typename RHS43 , typename RHS44>
        struct sub_t<
                     math::matrix4x4<LHS11,LHS12,LHS13,LHS14,
                                     LHS21,LHS22,LHS23,LHS24,
                                     LHS31,LHS32,LHS33,LHS34,
                                     LHS41,LHS42,LHS43,LHS44> , 
                
                     math::matrix4x4<RHS11,RHS12,RHS13,RHS14,
                                     RHS21,RHS22,RHS23,RHS24,
                                     RHS31,RHS32,RHS33,RHS34,
                                     RHS41,RHS42,RHS43,RHS44>
                    > : public tb::function<math::matrix4x4<tb::sub<LHS11,RHS11> , tb::sub<LHS12,RHS12> , tb::sub<LHS13,RHS13> , tb::sub<LHS14,RHS14> ,
                                                             tb::sub<LHS21,RHS21> , tb::sub<LHS22,RHS22> , tb::sub<LHS23,RHS23> , tb::sub<LHS24,RHS24> ,
                                                             tb::sub<LHS31,RHS31> , tb::sub<LHS32,RHS32> , tb::sub<LHS33,RHS33> , tb::sub<LHS34,RHS34> ,
                                                             tb::sub<LHS41,RHS41> , tb::sub<LHS42,RHS42> , tb::sub<LHS43,RHS43> , tb::sub<LHS44,RHS44>>>
        {};
        
        
        // matrix * scalar:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 , typename LHS14 ,
                 typename LHS21 , typename LHS22 , typename LHS23 , typename LHS24 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , typename LHS34 ,
                 typename LHS41 , typename LHS42 , typename LHS43 , typename LHS44 ,
        
                 typename RHS>
        
        struct mul_t< 
                     math::matrix4x4<LHS11,LHS12,LHS13,LHS14,
                                     LHS21,LHS22,LHS23,LHS24,
                                     LHS31,LHS32,LHS33,LHS34,
                                     LHS41,LHS42,LHS43,LHS44> , 
                     RHS
                    > : public tb::function<math::matrix4x4<tb::mul<LHS11,RHS> , tb::mul<LHS12,RHS> , tb::mul<LHS13,RHS> , tb::mul<LHS14,RHS> ,
                                                             tb::mul<LHS21,RHS> , tb::mul<LHS22,RHS> , tb::mul<LHS23,RHS> , tb::mul<LHS24,RHS> ,
                                                             tb::mul<LHS31,RHS> , tb::mul<LHS32,RHS> , tb::mul<LHS33,RHS> , tb::mul<LHS34,RHS> ,
                                                             tb::mul<LHS41,RHS> , tb::mul<LHS42,RHS> , tb::mul<LHS43,RHS> , tb::mul<LHS44,RHS>>>
        {};
        
        
        // scalar * matrix:
        
        template<typename RHS11 , typename RHS12 , typename RHS13 , typename RHS14 ,
                 typename RHS21 , typename RHS22 , typename RHS23 , typename RHS24 ,
                 typename RHS31 , typename RHS32 , typename RHS33 , typename RHS34 ,
                 typename RHS41 , typename RHS42 , typename RHS43 , typename RHS44 ,
        
                 typename LHS>
        
        struct mul_t<LHS,
                     math::matrix4x4<RHS11,RHS12,RHS13,RHS14,
                                     RHS21,RHS22,RHS23,RHS24,
                                     RHS31,RHS32,RHS33,RHS34,
                                     RHS41,RHS42,RHS43,RHS44>
                    > : public tb::function<math::matrix4x4<tb::mul<RHS11,LHS> , tb::mul<RHS12,LHS> , tb::mul<RHS13,LHS> , tb::mul<RHS14,LHS> ,
                                                             tb::mul<RHS21,LHS> , tb::mul<RHS22,LHS> , tb::mul<RHS23,LHS> , tb::mul<RHS24,LHS> ,
                                                             tb::mul<RHS31,LHS> , tb::mul<RHS32,LHS> , tb::mul<RHS33,LHS> , tb::mul<RHS34,LHS> ,
                                                             tb::mul<RHS41,LHS> , tb::mul<RHS42,LHS> , tb::mul<RHS43,LHS> , tb::mul<RHS44,LHS>>>
        {};
        
        
        // matrix / scalar:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 , typename LHS14 ,
                 typename LHS21 , typename LHS22 , typename LHS23 , typename LHS24 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , typename LHS34 ,
                 typename LHS41 , typename LHS42 , typename LHS43 , typename LHS44 ,
        
                 typename RHS>
        
        struct div_t< 
                     math::matrix4x4<LHS11,LHS12,LHS13,LHS14,
                                     LHS21,LHS22,LHS23,LHS24,
                                     LHS31,LHS32,LHS33,LHS34,
                                     LHS41,LHS42,LHS43,LHS44> , 
                     RHS
                    > : public tb::function<math::matrix4x4<tb::div<LHS11,RHS> , tb::div<LHS12,RHS> , tb::div<LHS13,RHS> , tb::div<LHS14,RHS> ,
                                                             tb::div<LHS21,RHS> , tb::div<LHS22,RHS> , tb::div<LHS23,RHS> , tb::div<LHS24,RHS> ,
                                                             tb::div<LHS31,RHS> , tb::div<LHS32,RHS> , tb::div<LHS33,RHS> , tb::div<LHS34,RHS> ,
                                                             tb::div<LHS41,RHS> , tb::div<LHS42,RHS> , tb::div<LHS43,RHS> , tb::div<LHS44,RHS>>>
        {};
        
        
        // matrix * matrix
        
        template<typename LHS11 , typename LHS12 , typename LHS13 , typename LHS14 ,
                 typename LHS21 , typename LHS22 , typename LHS23 , typename LHS24 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , typename LHS34 ,
                 typename LHS41 , typename LHS42 , typename LHS43 , typename LHS44 ,
                
                 typename RHS11 , typename RHS12 , typename RHS13 , typename RHS14 ,
                 typename RHS21 , typename RHS22 , typename RHS23 , typename RHS24 ,
                 typename RHS31 , typename RHS32 , typename RHS33 , typename RHS34 ,
                 typename RHS41 , typename RHS42 , typename RHS43 , typename RHS44>
        struct mul_t<
                     math::matrix4x4<LHS11,LHS12,LHS13,LHS14,
                                     LHS21,LHS22,LHS23,LHS24,
                                     LHS31,LHS32,LHS33,LHS34,
                                     LHS41,LHS42,LHS43,LHS44> , 
                
                     math::matrix4x4<RHS11,RHS12,RHS13,RHS14,
                                     RHS21,RHS22,RHS23,RHS24,
                                     RHS31,RHS32,RHS33,RHS34,
                                     RHS41,RHS42,RHS43,RHS44>
                    > : public tb::function<math::matrix4x4<decltype( LHS11()*RHS11() + LHS12()*RHS21() + LHS13()*RHS31() + LHS14()*LHS41() ) , //M11
                                                             decltype( LHS11()*RHS12() + LHS12()*RHS22() + LHS13()*RHS32() + LHS14()*LHS42() ) , //M12
                                                             decltype( LHS11()*RHS13() + LHS12()*RHS23() + LHS13()*RHS33() + LHS14()*LHS43() ) , //M13
                                                             decltype( LHS11()*RHS14() + LHS12()*RHS24() + LHS13()*RHS34() + LHS14()*LHS44() ) , //M14
                
                                                             decltype( LHS21()*RHS11() + LHS22()*RHS21() + LHS23()*RHS31() + LHS24()*LHS41() ) , //M21
                                                             decltype( LHS21()*RHS12() + LHS22()*RHS22() + LHS23()*RHS32() + LHS24()*LHS42() ) , //M22
                                                             decltype( LHS21()*RHS13() + LHS22()*RHS23() + LHS23()*RHS33() + LHS24()*LHS43() ) , //M23
                                                             decltype( LHS21()*RHS14() + LHS22()*RHS24() + LHS23()*RHS34() + LHS24()*LHS44() ) , //M23
                
                                                             decltype( LHS31()*RHS11() + LHS32()*RHS21() + LHS33()*RHS31() + LHS34()*LHS41() ) , //M31
                                                             decltype( LHS31()*RHS12() + LHS32()*RHS22() + LHS33()*RHS32() + LHS34()*LHS42() ) , //M32
                                                             decltype( LHS31()*RHS13() + LHS32()*RHS23() + LHS33()*RHS33() + LHS34()*LHS43() ) , //M33
                                                             decltype( LHS31()*RHS14() + LHS32()*RHS24() + LHS33()*RHS34() + LHS34()*LHS44() ) , //M34
                
                                                             decltype( LHS41()*RHS11() + LHS42()*RHS21() + LHS43()*RHS31() + LHS44()*LHS41() ) , //M41
                                                             decltype( LHS41()*RHS12() + LHS42()*RHS22() + LHS43()*RHS32() + LHS44()*LHS42() ) , //M42
                                                             decltype( LHS41()*RHS13() + LHS42()*RHS23() + LHS43()*RHS33() + LHS44()*LHS43() ) , //M43
                                                             decltype( LHS41()*RHS14() + LHS42()*RHS24() + LHS43()*RHS34() + LHS44()*LHS44() )>> //M44
        {};
        
        
        // matrix * vector (Apply transform):
        
        template<typename LHS11 , typename LHS12 , typename LHS13 , typename LHS14 ,
                 typename LHS21 , typename LHS22 , typename LHS23 , typename LHS24 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , typename LHS34 ,
                 typename LHS41 , typename LHS42 , typename LHS43 , typename LHS44 ,
        
                 typename X , typename Y , typename Z>
        
        struct mul_t< 
                     math::matrix4x4<LHS11,LHS12,LHS13,LHS14,
                                     LHS21,LHS22,LHS23,LHS24,
                                     LHS31,LHS32,LHS33,LHS34,
                                     LHS41,LHS42,LHS43,LHS44> , 
                math::vec3<X,Y,Z>
                    > : public tb::function<math::vec3<decltype( LHS11()*X() + LHS12()*Y() + LHS13()*Z() + LHS14() ),
                                                        decltype( LHS21()*X() + LHS22()*Y() + LHS23()*Z() + LHS24() ),
                                                        decltype( LHS31()*X() + LHS32()*Y() + LHS33()*Z() + LHS34() )>>
        {};
}
#endif	/* MATRIX4X4_HPP */

