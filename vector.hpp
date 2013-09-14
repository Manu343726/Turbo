/* 
 * File:   vector.hpp
 * Author: manu343726
 *
 * Created on 8 de septiembre de 2013, 23:23
 */

#ifndef VECTOR_HPP
#define	VECTOR_HPP

#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "fixed_point.hpp"
#include "to_string.hpp"

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
    struct square_length_t<math::vec2<X,Y>> : public mpl::function<decltype( X() * X() + Y() * Y() )> {};
    
    template<typename X , typename Y , typename Z>
    struct square_length_t<math::vec3<X,Y,Z>> : public mpl::function<decltype( X() * X() + Y() * Y() + Z() * Z() )> {};
    
    template<typename X , typename Y , typename Z , typename W>
    struct square_length_t<math::vec4<X,Y,Z,W>> : public mpl::function<decltype( X() * X() + Y() * Y() + Z() * Z() + W() * W() )> {};
    
    /* vectors length */
    
    //NOT IMPLEMENTED YET: Compile-time square-root needed
}

namespace mpl
{
    /* mpl::to_string specialization */
    
    template<typename X , typename Y>
    struct to_string_t<math::vec2<X,Y>>
    {
        operator std::string()
        {
            std::ostringstream os;
            os << "(" << mpl::to_string<X>() << "," << mpl::to_string<Y>() << ")";
            return os.str();
        }
    };
    
    template<typename X , typename Y , typename Z>
    struct to_string_t<math::vec3<X,Y,Z>>
    {
        operator std::string()
        {
            std::ostringstream os;
            os << "(" << mpl::to_string<X>() << "," << mpl::to_string<Y>() << "," << mpl::to_string<Z>() + ")";
            return os.str();
        }
    };
    
    template<typename X , typename Y , typename Z , typename W>
    struct to_string_t<math::vec4<X,Y,Z,W>>
    {
        operator std::string()
        {
            std::ostringstream os;
            os << "(" << mpl::to_string<X>() << "," << mpl::to_string<Y>() << "," << mpl::to_string<Z>() << "," << mpl::to_string<W>() << ")";
            return os.str();
        }
    };
    
    /* Arithmetic operations */
    
    //Addition:
    
    template<typename X1 , typename Y1 , typename X2 , typename Y2>
    struct add_t<math::vec2<X1,Y1>,math::vec2<X2,Y2>> : public mpl::function<math::vec2<mpl::add<X1,X2> , mpl::add<Y1,Y2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct add_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public mpl::function<math::vec3<mpl::add<X1,X2> , mpl::add<Y1,Y2> , mpl::add<Z1,Z2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename W1 , typename X2 , typename Y2 , typename Z2 , typename W2>
    struct add_t<math::vec4<X1,Y1,Z1,W1>,math::vec4<X2,Y2,Z2,W2>> : public mpl::function<math::vec4<mpl::add<X1,X2> , mpl::add<Y1,Y2> , mpl::add<Z1,Z2> , mpl::add<W1,W2>>> {};
    
    //Substraction:
    
    template<typename X1 , typename Y1 , typename X2 , typename Y2>
    struct sub_t<math::vec2<X1,Y1>,math::vec2<X2,Y2>> : public mpl::function<math::vec2<mpl::sub<X1,X2> , mpl::sub<Y1,Y2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct sub_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public mpl::function<math::vec3<mpl::sub<X1,X2> , mpl::sub<Y1,Y2> , mpl::sub<Z1,Z2>>> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename W1 , typename X2 , typename Y2 , typename Z2 , typename W2>
    struct sub_t<math::vec4<X1,Y1,Z1,W1>,math::vec4<X2,Y2,Z2,W2>> : public mpl::function<math::vec4<mpl::sub<X1,X2> , mpl::sub<Y1,Y2> , mpl::sub<Z1,Z2> , mpl::sub<W1,W2>>> {};

    //Scalar product:
    
    template<typename X1 , typename Y1 , typename X2 , typename Y2>
    struct mul_t<math::vec2<X1,Y1>,math::vec2<X2,Y2>> : public mpl::function<decltype( X1() * X2() + Y1() * Y2())> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename X2 , typename Y2 , typename Z2>
    struct mul_t<math::vec3<X1,Y1,Z1>,math::vec3<X2,Y2,Z2>> : public mpl::function<decltype( X1() * X2() + Y1() * Y2() + Z1() * Z2())> {};
    
    template<typename X1 , typename Y1 , typename Z1 , typename W1 , typename X2 , typename Y2 , typename Z2 , typename W2>
    struct mul_t<math::vec4<X1,Y1,Z1,W1>,math::vec4<X2,Y2,Z2,W2>> : public mpl::function<decltype( X1() * X2() + Y1() * Y2() + Z1() * Z2() + W1() * W2())> {};

    //Porduct:

    template<typename X , typename Y , typename RHS>
    struct mul_t<math::vec2<X,Y>,RHS> : public mpl::function<math::vec2<mpl::mul<X,RHS>,mpl::mul<Y,RHS>>> {};
    
    template<typename LHS , typename X , typename Y>
    struct mul_t<LHS,math::vec2<X,Y>> : public mpl::function<math::vec2<mpl::mul<X,LHS>,mpl::mul<Y,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename RHS>
    struct mul_t<math::vec3<X,Y,Z>,RHS> : public mpl::function<math::vec3<mpl::mul<X,RHS>,mpl::mul<Y,RHS>,mpl::mul<Z,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z>
    struct mul_t<LHS,math::vec3<X,Y,Z>> : public mpl::function<math::vec3<mpl::mul<X,LHS>,mpl::mul<Y,LHS>,mpl::mul<Z,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename W , typename RHS>
    struct mul_t<math::vec4<X,Y,Z,W>,RHS> : public mpl::function<math::vec4<mpl::mul<X,RHS>,mpl::mul<Y,RHS>,mpl::mul<Z,RHS>,mpl::mul<W,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z , typename W>
    struct mul_t<LHS,math::vec4<X,Y,Z,W>> : public mpl::function<math::vec4<mpl::mul<X,LHS>,mpl::mul<Y,LHS>,mpl::mul<Z,LHS>,mpl::mul<W,LHS>>> {};
    
    //Division:
    
    template<typename X , typename Y , typename RHS>
    struct div_t<math::vec2<X,Y>,RHS> : public mpl::function<math::vec2<mpl::div<X,RHS>,mpl::div<Y,RHS>>> {};
    
    template<typename LHS , typename X , typename Y>
    struct div_t<LHS,math::vec2<X,Y>> : public mpl::function<math::vec2<mpl::div<X,LHS>,mpl::div<Y,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename RHS>
    struct div_t<math::vec3<X,Y,Z>,RHS> : public mpl::function<math::vec3<mpl::div<X,RHS>,mpl::div<Y,RHS>,mpl::div<Z,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z>
    struct div_t<LHS,math::vec3<X,Y,Z>> : public mpl::function<math::vec3<mpl::div<X,LHS>,mpl::div<Y,LHS>,mpl::div<Z,LHS>>> {};
    
    template<typename X , typename Y , typename Z , typename W , typename RHS>
    struct div_t<math::vec4<X,Y,Z,W>,RHS> : public mpl::function<math::vec4<mpl::div<X,RHS>,mpl::div<Y,RHS>,mpl::div<Z,RHS>,mpl::div<W,RHS>>> {};
    
    template<typename LHS , typename X , typename Y , typename Z , typename W>
    struct div_t<LHS,math::vec4<X,Y,Z,W>> : public mpl::function<math::vec4<mpl::div<X,LHS>,mpl::div<Y,LHS>,mpl::div<Z,LHS>,mpl::div<W,LHS>>> {};
}

#endif	/* VECTOR_HPP */

