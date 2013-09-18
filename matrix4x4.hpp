/* 
 * File:   matrix4x4.hpp
 * Author: manu343726
 *
 * Created on 16 de septiembre de 2013, 20:19
 */

#ifndef MATRIX4X4_HPP
#define	MATRIX4X4_HPP

#include "operators.hpp"
#include "to_string.hpp"
#include "trigonometry.hpp"
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
        using unity4x4 = math::matrix4x4<mpl::one<T>  , mpl::zero<T> , mpl::zero<T> , mpl::zero<T> ,
                                         mpl::zero<T> , mpl::one<T>  , mpl::zero<T> , mpl::zero<T> ,
                                         mpl::zero<T> , mpl::zero<T> , mpl::one<T>  , mpl::zero<T> ,
                                         mpl::zero<T> , mpl::zero<T> , mpl::zero<T> , mpl::one<T>>;
        
        
        /* 2d transformations */
        
        template<typename X , typename Y , typename Z>
        struct translate_t<math::vec3<X,Y,Z>> : public mpl::function<math::matrix4x4<mpl::one<X>  , mpl::zero<X> , mpl::zero<X> , X ,
                                                                                     mpl::zero<X> , mpl::one<X>  , mpl::zero<X> , Y ,
                                                                                     mpl::zero<X> , mpl::zero<X> , mpl::one<X>  , Z ,
                                                                                     mpl::zero<X> , mpl::zero<X> , mpl::zero<X> , mpl::one<X>>>
        {};
        
        template<typename X , typename Y , typename Z>
        struct scale_t<math::vec3<X,Y,Z>> : public mpl::function<math::matrix4x4<      X      , mpl::zero<X> , mpl::zero<X> , mpl::zero<X> ,
                                                                                 mpl::zero<X> ,       Y      , mpl::zero<X> , mpl::zero<X> ,
                                                                                 mpl::zero<X> , mpl::zero<X> ,       Z      , mpl::zero<X> ,
                                                                                 mpl::zero<X> , mpl::zero<X> , mpl::zero<X> , mpl::one<X>>>
        {};
        
        
        template<typename ANGLE>
        struct rotate_t<
        {};
}

namespace mpl
{
        /* to string function specialization */
        
        template<typename M11 , typename M12 , typename M13 ,
                 typename M21 , typename M22 , typename M23 ,
                 typename M31 , typename M32 , typename M33>
        
        struct to_string_t<math::matrix4x4<M11,M12,M13,
                                           M21,M22,M23,
                                           M31,M32,M33>>
        {
            operator std::string()
            {
                std::ostringstream os;
                
                os << "|" << mpl::to_string<M11>() << "," << mpl::to_string<M12>() << "," << mpl::to_string<M13>() << "|" << std::endl;
                os << "|" << mpl::to_string<M21>() << "," << mpl::to_string<M22>() << "," << mpl::to_string<M23>() << "|" << std::endl;
                os << "|" << mpl::to_string<M31>() << "," << mpl::to_string<M32>() << "," << mpl::to_string<M33>() << "|";
                
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
                     math::matrix4x4<LHS11,LHS12,LHS13,
                                     LHS21,LHS22,LHS23,
                                     LHS31,LHS32,LHS33> , 
                
                     math::matrix4x4<RHS11,RHS12,RHS13,
                                     RHS21,RHS22,RHS23,
                                     RHS31,RHS32,RHS33>
                    > : public mpl::function<math::matrix4x4<mpl::add<LHS11,RHS11> , mpl::add<LHS12,RHS12> , mpl::add<LHS13,RHS13> , 
                                                             mpl::add<LHS21,RHS31> , mpl::add<LHS22,RHS22> , mpl::add<LHS23,RHS23> , 
                                                             mpl::add<LHS31,RHS21> , mpl::add<LHS32,RHS32> , mpl::add<LHS33,RHS33>>> 
        {};
        
        
        // matrix - matrix:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 ,
                 typename LHS21 , typename LHS22 , typename LHS23 ,
                 typename LHS31 , typename LHS32 , typename LHS33 ,
                
                 typename RHS11 , typename RHS12 , typename RHS13 ,
                 typename RHS21 , typename RHS22 , typename RHS23 ,
                 typename RHS31 , typename RHS32 , typename RHS33>
        struct sub_t<
                     math::matrix4x4<LHS11,LHS12,LHS13,
                                     LHS21,LHS22,LHS23,
                                     LHS31,LHS32,LHS33> , 
                
                     math::matrix4x4<RHS11,RHS12,RHS13,
                                     RHS21,RHS22,RHS23,
                                     RHS31,RHS32,RHS33>
                    > : public mpl::function<math::matrix4x4<mpl::sub<LHS11,RHS11> , mpl::sub<LHS12,RHS12> , mpl::sub<LHS13,RHS13> , 
                                                             mpl::sub<LHS21,RHS31> , mpl::sub<LHS22,RHS22> , mpl::sub<LHS23,RHS23> , 
                                                             mpl::sub<LHS31,RHS21> , mpl::sub<LHS32,RHS32> , mpl::sub<LHS33,RHS33>>> 
        {};
        
        
        // matrix * scalar:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 ,
                 typename LHS21 , typename LHS22 , typename LHS23 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , 
        
                 typename RHS>
        
        struct mul_t< 
                     math::matrix4x4<LHS11,LHS12,LHS13,
                                     LHS21,LHS22,LHS23,
                                     LHS31,LHS32,LHS33> , 
                     RHS
                    > : public mpl::function<math::matrix4x4<mpl::mul<LHS11,RHS> , mpl::mul<LHS12,RHS> , mpl::mul<LHS13,RHS> , 
                                                             mpl::mul<LHS21,RHS> , mpl::mul<LHS22,RHS> , mpl::mul<LHS23,RHS> , 
                                                             mpl::mul<LHS31,RHS> , mpl::mul<LHS32,RHS> , mpl::mul<LHS33,RHS>>> 
        {};
        
        
        // scalar * matrix:
        
        template<typename RHS11 , typename RHS12 , typename RHS13 ,
                 typename RHS21 , typename RHS22 , typename RHS23 ,
                 typename RHS31 , typename RHS32 , typename RHS33 , 
        
                 typename LHS>
        
        struct mul_t<LHS ,
                     math::matrix4x4<RHS11,RHS12,RHS13,
                                     RHS21,RHS22,RHS23,
                                     RHS31,RHS32,RHS33>
                    > : public mpl::function<math::matrix4x4<mpl::mul<RHS11,LHS> , mpl::mul<RHS12,LHS> , mpl::mul<RHS13,LHS> , 
                                                             mpl::mul<RHS21,LHS> , mpl::mul<RHS22,LHS> , mpl::mul<RHS23,LHS> , 
                                                             mpl::mul<RHS31,LHS> , mpl::mul<RHS32,LHS> , mpl::mul<RHS33,LHS>>> 
        {};
        
        
        //matrix / scalar:
        
        template<typename LHS11 , typename LHS12 , typename LHS13 ,
                 typename LHS21 , typename LHS22 , typename LHS23 ,
                 typename LHS31 , typename LHS32 , typename LHS33 , 
        
                 typename RHS>
        
        struct div_t< 
                     math::matrix4x4<LHS11,LHS12,LHS13,
                                     LHS21,LHS22,LHS23,
                                     LHS31,LHS32,LHS33> , 
                     RHS
                    > : public mpl::function<math::matrix4x4<decltype(LHS11()/RHS()) , decltype(LHS12()/RHS()) , mpl::div<LHS13,RHS> , 
                                                             mpl::div<LHS21,RHS> , mpl::div<LHS22,RHS> , mpl::div<LHS23,RHS> , 
                                                             mpl::div<LHS31,RHS> , mpl::div<LHS32,RHS> , mpl::div<LHS33,RHS>>> 
        {};
        
        
        // matrix * matrix
        
        template<typename LHS11 , typename LHS12 , typename LHS13 ,
                 typename LHS21 , typename LHS22 , typename LHS23 ,
                 typename LHS31 , typename LHS32 , typename LHS33 ,
                
                 typename RHS11 , typename RHS12 , typename RHS13 ,
                 typename RHS21 , typename RHS22 , typename RHS23 ,
                 typename RHS31 , typename RHS32 , typename RHS33>
        struct mul_t<
                     math::matrix4x4<LHS11,LHS12,LHS13,
                                     LHS21,LHS22,LHS23,
                                     LHS31,LHS32,LHS33> , 
                
                     math::matrix4x4<RHS11,RHS12,RHS13,
                                     RHS21,RHS22,RHS23,
                                     RHS31,RHS32,RHS33>
                    > : public mpl::function<math::matrix4x4<decltype( LHS11()*RHS11() + LHS12()*RHS21() + LHS13()*RHS31() ) , //M11
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
                     math::matrix4x4<LHS11,LHS12,LHS13,
                                     LHS21,LHS22,LHS23,
                                     LHS31,LHS32,LHS33> , 
                math::vec3<X,Y>
                    > : public mpl::function<math::vec3<decltype( LHS11()*X() + LHS12()*Y() + LHS13() ),
                                                        decltype( LHS21()*X() + LHS22()*Y() + LHS23() )>>
        {};
}
#endif	/* MATRIX4X4_HPP */

