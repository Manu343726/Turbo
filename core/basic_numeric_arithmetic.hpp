/* 
 * File:   basic_numeric_arithmetic.hpp
 * Author: manu343726
 *
 * Created on 7 de febrero de 2014, 20:19
 */

#ifndef BASIC_NUMERIC_ARITHMETIC_HPP
#define	BASIC_NUMERIC_ARITHMETIC_HPP

#include "value_t.hpp"
#include "arithmetic_functions.hpp"

namespace tb
{
    /* identities */
    
    template<typename T , T a>
    struct one_t<tb::value_t<T,a>> : public tb::function<tb::value_t<T,1>> {};
    
    template<typename T , T a>
    struct zero_t<tb::value_t<T,a>> : public tb::function<tb::value_t<T,0>> {};
    
    /* Arithmetic operators */
    
    template<typename T , typename U , T a , U b>
    struct add_t<tb::value_t<T,a>,tb::value_t<U,b>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , a + b>> {};
    
    template<typename T , typename U , T a , U b>
    struct sub_t<tb::value_t<T,a>,tb::value_t<U,b>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , a - b>> {};
    
    template<typename T , typename U , T a , U b>
    struct mul_t<tb::value_t<T,a>,tb::value_t<U,b>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , a * b>> {};
    
    template<typename T , typename U , T a , U b>
    struct div_t<tb::value_t<T,a>,tb::value_t<U,b>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , a / b>> {};
    
    
    /* logical operators */
    
    template<bool b>
    struct logical_not_t<tb::value_t<bool,b>> : public tb::function<tb::value_t<bool,!b>> {};
    
    template<bool lhs , bool rhs>
    struct logical_and_t<tb::value_t<bool,lhs>,tb::value_t<bool,rhs>> : public tb::function<tb::value_t<bool,lhs && rhs>> {};
    
    template<bool lhs , bool rhs>
    struct logical_or_t<tb::value_t<bool,lhs>,tb::value_t<bool,rhs>> : public tb::function<tb::value_t<bool,lhs || rhs>> {};
    
    template<bool lhs , bool rhs>
    struct logical_xor_t<tb::value_t<bool,lhs>,tb::value_t<bool,rhs>> : public tb::function<tb::value_t<bool,lhs ^ rhs>> {};
    
    /* comparison operators */
    
    template<typename T , typename U , T lhs , U rhs>
    struct equal_t<tb::value_t<T,lhs>,tb::value_t<U,rhs>> : public tb::function<tb::value_t<bool,lhs == rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct less_than_t<tb::value_t<T,lhs>,tb::value_t<U,rhs>> : public tb::function<tb::value_t<bool,(lhs < rhs)>> {};
    
    /* bitwise operators */
    
    template<typename T , T a>
    struct bitwise_not_t<tb::value_t<T,a>> : public tb::function<tb::value_t<T,~a>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_and_t<tb::value_t<T,lhs>,tb::value_t<U,rhs>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , lhs & rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_or_t<tb::value_t<T,lhs>,tb::value_t<U,rhs>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , lhs | rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_xor_t<tb::value_t<T,lhs>,tb::value_t<U,rhs>> : public tb::function<tb::value_t<typename std::common_type<T,U>::type , lhs ^ rhs>> {};
    
    template<typename T , T value , typename U , U shift>
    struct bitwise_leftshift_t<tb::value_t<T,value>,tb::value_t<U,shift>> : public tb::function<tb::value_t<T,( value << shift )>> {};
    
    template<typename T , T value , typename U , U shift>
    struct bitwise_rightshift_t<tb::value_t<T,value>,tb::value_t<U,shift>> : public tb::function<tb::value_t<T,( value >> shift )>> {};
}

#endif	/* BASIC_NUMERIC_ARITHMETIC_HPP */

