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

#ifndef VECTOR_HPP
#define	VECTOR_HPP

#include "core.hpp"
#include "fixed_point.hpp"
#include "to_string.hpp"
#include "sqrt.hpp"

#include <sstream>

namespace math
{
    /* vectors declarations */
    
    template<typename X , typename Y>
    struct vec2
    {
        using x = X;
        using y = Y;
    };
    
    template<typename X , typename Y , typename Z>
    struct vec3
    {
        using x = X;
        using y = Y;
        using z = Z;
    };
    
    template<typename X , typename Y , typename Z , typename W>
    struct vec4
    {
        using x = X;
        using y = Y;
        using z = Z;
        using w = W;
    };
    
    /* vectors square lenght */
    
    template<typename VECTOR>
    struct square_length_t;
    
    template<typename VECTOR>
    using square_length = typename math::square_length_t<VECTOR>::result;
    
    
    template<typename X , typename Y>
    struct square_length_t<math::vec2<X,Y>> : public tml::function<decltype( X() * X() + Y() * Y() )> {};
    
    template<typename X , typename Y , typename Z>
    struct square_length_t<math::vec3<X,Y,Z>> : public tml::function<decltype( X() * X() + Y() * Y() + Z() * Z() )> {};
    
    template<typename X , typename Y , typename Z , typename W>
    struct square_length_t<math::vec4<X,Y,Z,W>> : public tml::function<decltype( X() * X() + Y() * Y() + Z() * Z() + W() * W() )> {};
    
    template<typename VECTOR>
    using square_length = typename square_length_t<VECTOR>::result;
    
    /* vectors length */
    
    template<typename VECTOR>
    struct length_t;
    
    template<typename X , typename Y>
    struct length_t<math::vec2<X,Y>> : public tml::function<math::sqrt<decltype( X() * X() + Y() * Y() )>> {};
    
    template<typename X , typename Y , typename Z>
    struct length_t<math::vec3<X,Y,Z>> : public tml::function<math::sqrt<decltype( X() * X() + Y() * Y() + Z() * Z() )>> {};
    
    template<typename X , typename Y , typename Z , typename W>
    struct length_t<math::vec4<X,Y,Z,W>> : public tml::function<math::sqrt<decltype( X() * X() + Y() * Y() + Z() * Z() + W() * W() )>> {};
    
    template<typename VECTOR>
    using length = typename length_t<VECTOR>::result;
    
    template<typename VECTOR>
    using normalize = tml::div<VECTOR,math::length<VECTOR>>;
    
    /* 3d cross product */
    
    template<typename V1 , typename V2>
    struct cross_product_t;
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct cross_product_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public tml::function<math::vec3<decltype( Y1()*Z2()-Z1()*Y2() ) , 
                                                                                                        decltype( Z1()*X2()-X1()*Z2() ) , 
                                                                                                        decltype( X1()*Y2()-Y1()*X2() )>> {};
                                                                                                    
    template<typename V1 , typename V2>
    using cross_product = typename cross_product_t<V1,V2>::result;
}

namespace tml
{
    /* mpl::to_string specialization */
    
    template<typename X , typename Y>
    struct to_string_t<math::vec2<X,Y>>
    {
        operator std::string()
        {
            std::ostringstream os;
            os << "(" << tml::to_string<X>() << "," << tml::to_string<Y>() << ")";
            return os.str();
        }
    };
    
    template<typename X , typename Y , typename Z>
    struct to_string_t<math::vec3<X,Y,Z>>
    {
        operator std::string()
        {
            std::ostringstream os;
            os << "(" << tml::to_string<X>() << "," << tml::to_string<Y>() << "," << tml::to_string<Z>() + ")";
            return os.str();
        }
    };
    
    template<typename X , typename Y , typename Z , typename W>
    struct to_string_t<math::vec4<X,Y,Z,W>>
    {
        operator std::string()
        {
            std::ostringstream os;
            os << "(" << tml::to_string<X>() << "," << tml::to_string<Y>() << "," << tml::to_string<Z>() << "," << tml::to_string<W>() << ")";
            return os.str();
        }
    };
    
    /* Arithmetic operations */
    
    //Addition:
    
    template<typename X1 , typename Y1 , typename X2 , typename Y2>
    struct add_t<math::vec2<X1,Y1>,math::vec2<X2,Y2>> : public tml::function<math::vec2<tml::add<X1,X2> , tml::add<Y1,Y2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct add_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public tml::function<math::vec3<tml::add<X1,X2> , tml::add<Y1,Y2> , tml::add<Z1,Z2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename W1 , typename X2 , typename Y2 , typename Z2 , typename W2>
    struct add_t<math::vec4<X1,Y1,Z1,W1>,math::vec4<X2,Y2,Z2,W2>> : public tml::function<math::vec4<tml::add<X1,X2> , tml::add<Y1,Y2> , tml::add<Z1,Z2> , tml::add<W1,W2>>> {};
    
    //Substraction:
    
    template<typename X1 , typename Y1 , typename X2 , typename Y2>
    struct sub_t<math::vec2<X1,Y1>,math::vec2<X2,Y2>> : public tml::function<math::vec2<tml::sub<X1,X2> , tml::sub<Y1,Y2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct sub_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public tml::function<math::vec3<tml::sub<X1,X2> , tml::sub<Y1,Y2> , tml::sub<Z1,Z2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename W1 , typename X2 , typename Y2 , typename Z2 , typename W2>
    struct sub_t<math::vec4<X1,Y1,Z1,W1>,math::vec4<X2,Y2,Z2,W2>> : public tml::function<math::vec4<tml::sub<X1,X2> , tml::sub<Y1,Y2> , tml::sub<Z1,Z2> , tml::sub<W1,W2>>> {};

    //Scalar product:
    
    template<typename X1 , typename Y1 , typename X2 , typename Y2>
    struct mul_t<math::vec2<X1,Y1>,math::vec2<X2,Y2>> : public tml::function<decltype( X1() * X2() + Y1() * Y2())> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct mul_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public tml::function<decltype( X1() * X2() + Y1() * Y2() + Z1() * Z2())> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename W1 , typename X2 , typename Y2 , typename Z2 , typename W2>
    struct mul_t<math::vec4<X1,Y1,Z1,W1>,math::vec4<X2,Y2,Z2,W2>> : public tml::function<decltype( X1() * X2() + Y1() * Y2() + Z1() * Z2() + W1() * W2())> {};

    //Porduct:

    template<typename X , typename Y , typename RHS>
    struct mul_t<math::vec2<X,Y>,RHS> : public tml::function<math::vec2<tml::mul<X,RHS>,tml::mul<Y,RHS>>> {};
    
    template<typename LHS , typename X , typename Y>
    struct mul_t<LHS,math::vec2<X,Y>> : public tml::function<math::vec2<tml::mul<X,LHS>,tml::mul<Y,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename RHS>
    struct mul_t<math::vec3<X,Y,Z>,RHS> : public tml::function<math::vec3<tml::mul<X,RHS>,tml::mul<Y,RHS>,tml::mul<Z,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z>
    struct mul_t<LHS,math::vec3<X,Y,Z>> : public tml::function<math::vec3<tml::mul<X,LHS>,tml::mul<Y,LHS>,tml::mul<Z,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename W , typename RHS>
    struct mul_t<math::vec4<X,Y,Z,W>,RHS> : public tml::function<math::vec4<tml::mul<X,RHS>,tml::mul<Y,RHS>,tml::mul<Z,RHS>,tml::mul<W,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z , typename W>
    struct mul_t<LHS,math::vec4<X,Y,Z,W>> : public tml::function<math::vec4<tml::mul<X,LHS>,tml::mul<Y,LHS>,tml::mul<Z,LHS>,tml::mul<W,LHS>>> {};
    
    //Division:
    
    template<typename X , typename Y , typename RHS>
    struct div_t<math::vec2<X,Y>,RHS> : public tml::function<math::vec2<tml::div<X,RHS>,tml::div<Y,RHS>>> {};
    
    template<typename LHS , typename X , typename Y>
    struct div_t<LHS,math::vec2<X,Y>> : public tml::function<math::vec2<tml::div<X,LHS>,tml::div<Y,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename RHS>
    struct div_t<math::vec3<X,Y,Z>,RHS> : public tml::function<math::vec3<tml::div<X,RHS>,tml::div<Y,RHS>,tml::div<Z,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z>
    struct div_t<LHS,math::vec3<X,Y,Z>> : public tml::function<math::vec3<tml::div<X,LHS>,tml::div<Y,LHS>,tml::div<Z,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename W , typename RHS>
    struct div_t<math::vec4<X,Y,Z,W>,RHS> : public tml::function<math::vec4<tml::div<X,RHS>,tml::div<Y,RHS>,tml::div<Z,RHS>,tml::div<W,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z , typename W>
    struct div_t<LHS,math::vec4<X,Y,Z,W>> : public tml::function<math::vec4<tml::div<X,LHS>,tml::div<Y,LHS>,tml::div<Z,LHS>,tml::div<W,LHS>>> {};
}

#endif	/* VECTOR_HPP */

