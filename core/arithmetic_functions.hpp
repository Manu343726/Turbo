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

#pragma once

#include "function.hpp"
#include "basic_types.hpp"
#include "boolean_predicates.hpp"

#include <type_traits>

namespace tb
{   
    /* Metafunction */
    

    
    /* Identities */
    
    template<typename T>
    struct zero_t : public tb::function<tb::no_type> {}; //default
    
    template<typename T>
    struct one_t : public tb::function<T> {}; //default
    
    template<typename T>
    using zero = tb::result_of<tb::zero_t<T>>;
    
    template<typename T>
    using one = tb::result_of<tb::one_t<T>>;
    
    

    /* Logical functions */
    
    template<typename OP>
    struct logical_not_t;

    template<typename LHS , typename RHS>
    struct logical_or_t;
    
    template<typename LHS , typename RHS>
    struct logical_and_t;

    template<typename LHS , typename RHS>
    struct logical_xor_t;

    template<typename OP>
    using logical_not = tb::result_of<tb::logical_not_t<OP>>;

    template<typename LHS , typename RHS>
    using logical_or = tb::result_of<tb::logical_or_t<LHS,RHS>>;
    
    template<typename LHS , typename RHS>
    using logical_and = tb::result_of<tb::logical_and_t<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using logical_xor  = tb::result_of<tb::logical_xor_t<LHS,RHS>>;
    
    


    /* Bitwise functions */
    
    template<typename OP , typename SHIFT>
    struct bitwise_leftshift_t;

    template<typename OP , typename SHIFT>
    struct bitwise_rightshift_t;

    template<typename OP>
    struct bitwise_not_t;

    template<typename LHS , typename RHS>
    struct bitwise_or_t;

    template<typename LHS , typename RHS>
    struct bitwise_and_t;

    template<typename LHS , typename RHS>
    struct bitwise_xor_t;

    template<typename OP , typename SHIFT>
    using bitwise_leftshift = typename tb::bitwise_leftshift_t<OP,SHIFT>::result;

    template<typename OP , typename SHIFT>
    using bitwise_rightshift = typename tb::bitwise_rightshift_t<OP,SHIFT>::result;

    template<typename OP>
    using bitwise_not = typename tb::bitwise_not_t<OP>::result;

    template<typename LHS , typename RHS>
    using bitwise_or = typename tb::bitwise_or_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bitwise_and = typename tb::bitwise_and_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bitwise_xor = typename tb::bitwise_xor_t<LHS,RHS>::result;


    /* Comparison functions */

    template<typename LHS , typename RHS>
    struct equal_t : tb::false_function<> {};
    
    template<typename T>
    struct equal_t<T,T> : tb::true_function<> {};

    template<typename LHS , typename RHS>
    struct less_than_t;
    
    template<typename LHS , typename RHS>
    using equal = typename tb::equal_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using not_equal = tb::logical_not<tb::equal<LHS,RHS>>;
    
    template<typename LHS , typename RHS>
    using less_than = typename tb::less_than_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bigger_than = tb::less_than<RHS,LHS>;

    template<typename LHS , typename RHS>
    using less_or_equal = tb::logical_not<tb::bigger_than<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using bigger_or_equal = tb::logical_not<tb::less_than<LHS,RHS>>;


    /* Arithmetic functions */
    
    template<typename LHS , typename RHS>
    struct add_t;

    template<typename LHS , typename RHS>
    struct sub_t;

    template<typename LHS , typename RHS>
    struct mul_t;

    template<typename LHS , typename RHS>
    struct div_t;
    
    template<typename LHS , typename RHS>
    using add = typename tb::add_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using sub = typename tb::sub_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using mul = typename tb::mul_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using div = typename tb::div_t<LHS,RHS>::result;
    
    
    template<typename VALUE>
    using opposite = tb::sub<tb::zero<VALUE>,VALUE>;

    template<typename VALUE>
    using increment = tb::add<VALUE,tb::one<VALUE>>;

    template<typename VALUE>
    using decrement = tb::sub<VALUE,tb::one<VALUE>>;

    template<typename BASE , typename EXPONENT>
    struct pow : public tb::mul<BASE , pow<BASE,tb::decrement<EXPONENT>>> {};

    template<typename BASE>
    struct pow<BASE,zero<BASE>> : public tb::one<BASE> {};

    template<typename BASE>
    using square = tb::mul<BASE,BASE>;
}
	/* OPERATORS_HPP */

