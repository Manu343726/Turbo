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

#include <type_traits> //std::enable_if

namespace mpl
{
#define expr_result decltype
    
    template<typename LHS , typename RHS>
    typename std::enable_if<
   !std::is_same<
       void,
       typename mpl::add_t<LHS,RHS>::result
    >::value,
    mpl::add<LHS,RHS>
>::type
    operator+(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::sub<LHS,RHS> operator-(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::mul<LHS,RHS> operator*(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::div<LHS,RHS> operator/(const LHS& , const RHS&);
    
    
    
    
    template<typename LHS , typename RHS>
    mpl::logical_and<LHS,RHS> operator&&(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::logical_or<LHS,RHS>  operator||(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::logical_xor<LHS,RHS> operator^(const LHS& , const RHS&);
    
    template<typename T>
    mpl::logical_not<T>       operator!(const T&);
    
    
    
    
    template<typename LHS , typename RHS>
    mpl::equal<LHS,RHS>           operator==(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::not_equal<LHS,RHS>       operator!=(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::bigger_than<LHS,RHS>     operator>(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::less_than<LHS,RHS>       operator<(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::bigger_or_equal<LHS,RHS> operator>=(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    mpl::less_or_equal<LHS,RHS>   operator<=(const LHS& , const RHS&);
    
    
}

#endif	/* EXPRESSIONS_HPP */

