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

namespace
{
#define ENABLE_ONLY_FOR_SPECIALIZED_BINARY(result_metafunction) typename std::enable_if<                                           \
                                                                                        !std::is_same<                             \
                                                                                                      void,                        \
                                                                                                      result_metafunction<LHS,RHS> \
                                                                                                     >::value,                     \
                                                                                        result_metafunction<LHS,RHS>               \
                                                                                       >::type         
    
#define ENABLE_ONLY_FOR_SPECIALIZED_UNARY(result_metafunction) typename std::enable_if<                                     \
                                                                                       !std::is_same<                       \
                                                                                                     void,                  \
                                                                                                     result_metafunction<T> \
                                                                                                    >::value,               \
                                                                                       result_metafunction<T>               \
                                                                                      >::type   
}

namespace mpl
{
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::add) operator+(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::sub) operator-(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::mul) operator*(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::div) operator/(const LHS& , const RHS&);
    
    
    
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::bitwise_leftshift)  operator<<(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::bitwise_rightshift) operator>>(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::bitwise_and) operator&(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::bitwise_or)  operator|(const LHS& , const RHS&);
    
    template<typename T>
    ENABLE_ONLY_FOR_SPECIALIZED_UNARY(mpl::bitwise_not)  operator~(const T&);
    
    
    
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::logical_and) operator&&(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::logical_or)  operator||(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::logical_xor) operator^(const LHS& , const RHS&);
    
    template<typename T>
    ENABLE_ONLY_FOR_SPECIALIZED_UNARY(mpl::logical_not)  operator!(const T&);
    
    
    
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::equal)           operator==(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::not_equal)       operator!=(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::bigger_than)     operator>(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::less_than)       operator<(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::bigger_or_equal) operator>=(const LHS& , const RHS&);
    
    template<typename LHS , typename RHS>
    ENABLE_ONLY_FOR_SPECIALIZED_BINARY(mpl::less_or_equal)   operator<=(const LHS& , const RHS&);
    
    
}

#endif	/* EXPRESSIONS_HPP */

