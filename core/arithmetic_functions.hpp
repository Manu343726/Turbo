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

namespace tml
{   
    /* Metafunction */
    

    
    /* Identities */
    
    template<typename T>
    struct zero_t : public tml::function<tml::no_type> {}; //default
    
    template<typename T>
    struct one_t : public tml::function<T> {}; //default
    
    template<typename T>
    using zero = tml::result_of<tml::zero_t<T>>;
    
    template<typename T>
    using one = tml::result_of<tml::one_t<T>>;
    
    

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
    using logical_not = tml::result_of<tml::logical_not_t<OP>>;

    template<typename LHS , typename RHS>
    using logical_or = tml::result_of<tml::logical_or_t<LHS,RHS>>;
    
    template<typename LHS , typename RHS>
    using logical_and = tml::result_of<tml::logical_and_t<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using logical_xor  = tml::result_of<tml::logical_xor_t<LHS,RHS>>;
    
    


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
    using bitwise_leftshift = typename tml::bitwise_leftshift_t<OP,SHIFT>::result;

    template<typename OP , typename SHIFT>
    using bitwise_rightshift = typename tml::bitwise_rightshift_t<OP,SHIFT>::result;

    template<typename OP>
    using bitwise_not = typename tml::bitwise_not_t<OP>::result;

    template<typename LHS , typename RHS>
    using bitwise_or = typename tml::bitwise_or_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bitwise_and = typename tml::bitwise_and_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bitwise_xor = typename tml::bitwise_xor_t<LHS,RHS>::result;


    /* Comparison functions */

    template<typename LHS , typename RHS>
    struct equal_t : tml::false_function<> {};
    
    template<typename T>
    struct equal_t<T,T> : tml::true_function<> {};

    template<typename LHS , typename RHS>
    struct less_than_t;
    
    template<typename LHS , typename RHS>
    using equal = typename tml::equal_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using not_equal = tml::logical_not<tml::equal<LHS,RHS>>;
    
    template<typename LHS , typename RHS>
    using less_than = typename tml::less_than_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bigger_than = tml::less_than<RHS,LHS>;

    template<typename LHS , typename RHS>
    using less_or_equal = tml::logical_not<tml::bigger_than<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using bigger_or_equal = tml::logical_not<tml::less_than<LHS,RHS>>;


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
    using add = typename tml::add_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using sub = typename tml::sub_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using mul = typename tml::mul_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using div = typename tml::div_t<LHS,RHS>::result;
    
    
    template<typename VALUE>
    using opposite = tml::sub<tml::zero<VALUE>,VALUE>;

    template<typename VALUE>
    using increment = tml::add<VALUE,tml::one<VALUE>>;

    template<typename VALUE>
    using decrement = tml::sub<VALUE,tml::one<VALUE>>;

    template<typename BASE , typename EXPONENT>
    struct pow : public tml::mul<BASE , pow<BASE,tml::decrement<EXPONENT>>> {};

    template<typename BASE>
    struct pow<BASE,zero<BASE>> : public tml::one<BASE> {};

    template<typename BASE>
    using square = tml::mul<BASE,BASE>;
}
	/* OPERATORS_HPP */

