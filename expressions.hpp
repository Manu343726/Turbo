/* 
 * File:   expressions.hpp
 * Author: paradise
 *
 * Created on 3 de septiembre de 2013, 16:25
 */

#ifndef EXPRESSIONS_HPP
#define	EXPRESSIONS_HPP

#include "traits.hpp"
#include "operators.hpp"

namespace mpl
{
#define expr_result decltype
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::add<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator+(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::sub<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator-(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::mul<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator*(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::div<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator/(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    
    
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::logical_and<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator&&(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::logical_or<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>>  operator||(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::logical_xor<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator^(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T a>
    mpl::logical_not<mpl::value_t<T,a>>                       operator!(const mpl::value_t<T,a>&);
    
    
    
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::equal<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>>           operator==(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::not_equal<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>>       operator!=(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::bigger_than<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>>     operator>(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::less_than<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>>       operator<(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::bigger_or_equal<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> operator>=(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    template<typename T , T lhs , typename U , U rhs>
    mpl::less_or_equal<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>>   operator<=(const mpl::value_t<T,lhs>& , const mpl::value_t<U,rhs>&);
    
    
}

#endif	/* EXPRESSIONS_HPP */

