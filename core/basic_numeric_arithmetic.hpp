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

namespace tml
{
    /* identities */
    
    template<typename T , T a>
    struct one_t<tml::value_t<T,a>> : public tml::function<tml::value_t<T,1>> {};
    
    template<typename T , T a>
    struct zero_t<tml::value_t<T,a>> : public tml::function<tml::value_t<T,0>> {};
    
    /* Arithmetic operators */
    
    template<typename T , typename U , T a , U b>
    struct add_t<tml::value_t<T,a>,tml::value_t<U,b>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , a + b>> {};
    
    template<typename T , typename U , T a , U b>
    struct sub_t<tml::value_t<T,a>,tml::value_t<U,b>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , a - b>> {};
    
    template<typename T , typename U , T a , U b>
    struct mul_t<tml::value_t<T,a>,tml::value_t<U,b>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , a * b>> {};
    
    template<typename T , typename U , T a , U b>
    struct div_t<tml::value_t<T,a>,tml::value_t<U,b>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , a / b>> {};
    
    
    /* logical operators */
    
    template<bool b>
    struct logical_not_t<tml::value_t<bool,b>> : public tml::function<tml::value_t<bool,!b>> {};
    
    template<bool lhs , bool rhs>
    struct logical_and_t<tml::value_t<bool,lhs>,tml::value_t<bool,rhs>> : public tml::function<tml::value_t<bool,lhs && rhs>> {};
    
    template<bool lhs , bool rhs>
    struct logical_or_t<tml::value_t<bool,lhs>,tml::value_t<bool,rhs>> : public tml::function<tml::value_t<bool,lhs || rhs>> {};
    
    template<bool lhs , bool rhs>
    struct logical_xor_t<tml::value_t<bool,lhs>,tml::value_t<bool,rhs>> : public tml::function<tml::value_t<bool,lhs ^ rhs>> {};
    
    /* comparison operators */
    
    template<typename T , typename U , T lhs , U rhs>
    struct equal_t<tml::value_t<T,lhs>,tml::value_t<U,rhs>> : public tml::function<tml::value_t<bool,lhs == rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct less_than_t<tml::value_t<T,lhs>,tml::value_t<U,rhs>> : public tml::function<tml::value_t<bool,(lhs < rhs)>> {};
    
    /* bitwise operators */
    
    template<typename T , T a>
    struct bitwise_not_t<tml::value_t<T,a>> : public tml::function<tml::value_t<T,~a>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_and_t<tml::value_t<T,lhs>,tml::value_t<U,rhs>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , lhs & rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_or_t<tml::value_t<T,lhs>,tml::value_t<U,rhs>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , lhs | rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_xor_t<tml::value_t<T,lhs>,tml::value_t<U,rhs>> : public tml::function<tml::value_t<typename std::common_type<T,U>::type , lhs ^ rhs>> {};
    
    template<typename T , T value , typename U , U shift>
    struct bitwise_leftshift_t<tml::value_t<T,value>,tml::value_t<U,shift>> : public tml::function<tml::value_t<T,( value << shift )>> {};
    
    template<typename T , T value , typename U , U shift>
    struct bitwise_rightshift_t<tml::value_t<T,value>,tml::value_t<U,shift>> : public tml::function<tml::value_t<T,( value >> shift )>> {};
}

#endif	/* BASIC_NUMERIC_ARITHMETIC_HPP */

