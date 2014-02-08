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

#ifndef MATRIX3X3_HPP
#define	MATRIX3X3_HPP

#include "core.hpp"
#include "to_string.hpp"
#include "trigonometry.hpp"
#include "vector.hpp"
#include "transformations.hpp"

#include <sstream>
#include <string>


namespace math
{
    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>
    struct matrix3x3
    {
        using m11 = M11; using m12 = M12; using m13 = M13;
        using m21 = M21; using m22 = M22; using m23 = M23;
        using m31 = M31; using m32 = M32; using m33 = M33;
    };


    /* unitary matrix */

    template<typename T>
    using unity3x3 = math::matrix3x3<tb::one<T>  , tb::zero<T> , tb::zero<T> ,
                                     tb::zero<T> , tb::one<T>  , tb::zero<T> ,
                                     tb::zero<T> , tb::zero<T> , tb::one<T>>;


    /* 2d transformations */

    template<typename X , typename Y>
    struct translate_t<math::vec2<X,Y>> : public tb::function<math::matrix3x3<tb::one<X>  , tb::zero<X> , X ,
                                                                               tb::zero<X> , tb::one<X>  , Y ,
                                                                               tb::zero<X> , tb::zero<X> , tb::one<X>>>
    {};

    template<typename SCALE>
    struct scale_t : public tb::function<math::matrix3x3<       SCALE     , tb::zero<SCALE> , tb::zero<SCALE> ,
                                                          tb::zero<SCALE> ,       SCALE      , tb::zero<SCALE> ,
                                                          tb::zero<SCALE> , tb::zero<SCALE> ,       SCALE>>
    {};

    template<typename X , typename Y>
    struct scale_t<math::vec2<X,Y>> : public tb::function<math::matrix3x3<      X      , tb::zero<X> , tb::zero<X> ,
                                                                           tb::zero<X> ,       Y      , tb::zero<X> ,
                                                                           tb::zero<X> , tb::zero<X> , tb::one<X>>>
    {};

    template<typename ANGLE,typename TRIGONOMETRY_APROX_TERMS>
    struct rotate_t<ANGLE,math::rotation_2d,TRIGONOMETRY_APROX_TERMS> 
    : public tb::function<math::matrix3x3<math::cos<ANGLE,TRIGONOMETRY_APROX_TERMS>                , math::sin<ANGLE,TRIGONOMETRY_APROX_TERMS>  , tb::zero<ANGLE> ,
                                           tb::opposite<math::sin<ANGLE,TRIGONOMETRY_APROX_TERMS>> , math::cos<ANGLE,TRIGONOMETRY_APROX_TERMS>  , tb::zero<ANGLE> ,
                                           tb::zero<ANGLE>                                         ,               tb::zero<ANGLE>             , tb::one<ANGLE>>> 
    {};
        
        

}

namespace tb
{
    /* to string function specialization */

    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>

    struct to_string_t<math::matrix3x3<M11,M12,M13,
                                       M21,M22,M23,
                                       M31,M32,M33>>
    {
        operator std::string()
        {
            std::ostringstream os;

            os << "|" << tb::to_string<M11>() << "," << tb::to_string<M12>() << "," << tb::to_string<M13>() << "|" << std::endl;
            os << "|" << tb::to_string<M21>() << "," << tb::to_string<M22>() << "," << tb::to_string<M23>() << "|" << std::endl;
            os << "|" << tb::to_string<M31>() << "," << tb::to_string<M32>() << "," << tb::to_string<M33>() << "|";

            return os.str();
        }
    };



    /* Arithmetic functions */


    // matrix + matrix:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 ,

             typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33>
    struct add_t<
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 

                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tb::function<math::matrix3x3<tb::add<LHS11,RHS11> , tb::add<LHS12,RHS12> , tb::add<LHS13,RHS13> , 
                                                         tb::add<LHS21,RHS21> , tb::add<LHS22,RHS22> , tb::add<LHS23,RHS23> , 
                                                         tb::add<LHS31,RHS31> , tb::add<LHS32,RHS32> , tb::add<LHS33,RHS33>>> 
    {};


    // matrix - matrix:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 ,

             typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33>
    struct sub_t<
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 

                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tb::function<math::matrix3x3<tb::sub<LHS11,RHS11> , tb::sub<LHS12,RHS12> , tb::sub<LHS13,RHS13> , 
                                                         tb::sub<LHS21,RHS21> , tb::sub<LHS22,RHS22> , tb::sub<LHS23,RHS23> , 
                                                         tb::sub<LHS31,RHS31> , tb::sub<LHS32,RHS32> , tb::sub<LHS33,RHS33>>> 
    {};


    // matrix * scalar:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 , 

             typename RHS>

    struct mul_t< 
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 
                 RHS
                > : public tb::function<math::matrix3x3<tb::mul<LHS11,RHS> , tb::mul<LHS12,RHS> , tb::mul<LHS13,RHS> , 
                                                         tb::mul<LHS21,RHS> , tb::mul<LHS22,RHS> , tb::mul<LHS23,RHS> , 
                                                         tb::mul<LHS31,RHS> , tb::mul<LHS32,RHS> , tb::mul<LHS33,RHS>>> 
    {};


    // scalar * matrix:

    template<typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33 , 

             typename LHS>

    struct mul_t<LHS ,
                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tb::function<math::matrix3x3<tb::mul<RHS11,LHS> , tb::mul<RHS12,LHS> , tb::mul<RHS13,LHS> , 
                                                         tb::mul<RHS21,LHS> , tb::mul<RHS22,LHS> , tb::mul<RHS23,LHS> , 
                                                         tb::mul<RHS31,LHS> , tb::mul<RHS32,LHS> , tb::mul<RHS33,LHS>>> 
    {};


    //matrix / scalar:

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 , 

             typename RHS>

    struct div_t< 
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 
                 RHS
                > : public tb::function<math::matrix3x3<decltype(LHS11()/RHS()) , decltype(LHS12()/RHS()) , tb::div<LHS13,RHS> , 
                                                         tb::div<LHS21,RHS> , tb::div<LHS22,RHS> , tb::div<LHS23,RHS> , 
                                                         tb::div<LHS31,RHS> , tb::div<LHS32,RHS> , tb::div<LHS33,RHS>>> 
    {};


    // matrix * matrix

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 ,

             typename RHS11 , typename RHS12 , typename RHS13 ,
             typename RHS21 , typename RHS22 , typename RHS23 ,
             typename RHS31 , typename RHS32 , typename RHS33>
    struct mul_t<
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 

                 math::matrix3x3<RHS11,RHS12,RHS13,
                                 RHS21,RHS22,RHS23,
                                 RHS31,RHS32,RHS33>
                > : public tb::function<math::matrix3x3<decltype( LHS11()*RHS11() + LHS12()*RHS21() + LHS13()*RHS31() ) , //M11
                                                         decltype( LHS11()*RHS12() + LHS12()*RHS22() + LHS13()*RHS32() ) , //M12
                                                         decltype( LHS11()*RHS13() + LHS12()*RHS23() + LHS13()*RHS33() ) , //M13

                                                         decltype( LHS21()*RHS11() + LHS22()*RHS21() + LHS23()*RHS31() ) , //M21
                                                         decltype( LHS21()*RHS12() + LHS22()*RHS22() + LHS23()*RHS32() ) , //M22
                                                         decltype( LHS21()*RHS13() + LHS22()*RHS23() + LHS23()*RHS33() ) , //M23

                                                         decltype( LHS31()*RHS11() + LHS32()*RHS21() + LHS33()*RHS31() ) , //M31
                                                         decltype( LHS31()*RHS12() + LHS32()*RHS22() + LHS33()*RHS32() ) , //M32
                                                         decltype( LHS31()*RHS13() + LHS32()*RHS23() + LHS33()*RHS33() )>> //M33
    {};


    // matrix * vector (Apply transform):

    template<typename LHS11 , typename LHS12 , typename LHS13 ,
             typename LHS21 , typename LHS22 , typename LHS23 ,
             typename LHS31 , typename LHS32 , typename LHS33 , 

             typename X , typename Y>

    struct mul_t< 
                 math::matrix3x3<LHS11,LHS12,LHS13,
                                 LHS21,LHS22,LHS23,
                                 LHS31,LHS32,LHS33> , 
            math::vec2<X,Y>
                > : public tb::function<math::vec2<decltype( LHS11()*X() + LHS12()*Y() + LHS13() ),
                                                    decltype( LHS21()*X() + LHS22()*Y() + LHS23() )>>
    {};
    
    
    
    /* mpl::to_list specialization: */
    
    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>
    struct to_list_t<math::matrix3x3<M11,M12,M13,
                                     M21,M22,M23,
                                     M31,M32,M33>>
    : public tb::function<tb::list<M11,M12,M13,M21,M22,M23,M31,M32,M33>> 
    {};
    
    
    /* mpl::from_list specialization: */
    
    template<typename M11 , typename M12 , typename M13 ,
             typename M21 , typename M22 , typename M23 ,
             typename M31 , typename M32 , typename M33>
    struct from_list_t<tb::list<M11,M12,M13,M21,M22,M23,M31,M32,M33>>
    : public tb::function<math::matrix3x3<M11,M12,M13,
                                           M21,M22,M23,
                                           M31,M32,M33>
                          >
    {};
    
}
#endif	/* MATRIX3X3_HPP */

