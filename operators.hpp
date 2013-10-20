/* 
 * File:   functions.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 20:39
 */

#pragma once

#include <type_traits>

namespace mpl
{
    /* unmeaning sentinel type */
    
    struct no_type {};
    
    /* Metafunction */
    
    template<typename RESULT>
    struct function : public RESULT
    {
        using result = RESULT;
    };
    
    template<typename FUNCTION>
    using result_of = typename FUNCTION::result;
    
    /* Identities */
    
    template<typename T>
    struct zero_t : public mpl::function<mpl::no_type> {}; //default
    
    template<typename T>
    struct one_t : public mpl::function<T> {}; //default
    
    template<typename T>
    using zero = mpl::result_of<mpl::zero_t<T>>;
    
    template<typename T>
    using one = mpl::result_of<mpl::one_t<T>>;
    
    

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
    using logical_not = mpl::result_of<mpl::logical_not_t<OP>>;

    template<typename LHS , typename RHS>
    using logical_or = mpl::result_of<mpl::logical_or_t<LHS,RHS>>;
    
    template<typename LHS , typename RHS>
    using logical_and = mpl::result_of<mpl::logical_and_t<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using logical_xor  = mpl::result_of<mpl::logical_xor_t<LHS,RHS>>;
    
    


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
    using bitwise_leftshift = typename mpl::bitwise_leftshift_t<OP,SHIFT>::result;

    template<typename OP , typename SHIFT>
    using bitwise_rightshift = typename mpl::bitwise_rightshift_t<OP,SHIFT>::result;

    template<typename OP>
    using bitwise_not = typename mpl::bitwise_not_t<OP>::result;

    template<typename LHS , typename RHS>
    using bitwise_or = typename mpl::bitwise_or_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bitwise_and = typename mpl::bitwise_and_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bitwise_xor = typename mpl::bitwise_xor_t<LHS,RHS>::result;


    /* Comparison functions */

    template<typename LHS , typename RHS>
    struct equal_t;

    template<typename LHS , typename RHS>
    struct less_than_t;
    
    template<typename LHS , typename RHS>
    using equal = typename mpl::equal_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using not_equal = mpl::logical_not<mpl::equal<LHS,RHS>>;
    
    template<typename LHS , typename RHS>
    using less_than = typename mpl::less_than_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bigger_than = mpl::less_than<RHS,LHS>;

    template<typename LHS , typename RHS>
    using less_or_equal = mpl::logical_not<mpl::bigger_than<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using bigger_or_equal = mpl::logical_not<mpl::less_than<LHS,RHS>>;


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
    using add = typename mpl::add_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using sub = typename mpl::sub_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using mul = typename mpl::mul_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using div = typename mpl::div_t<LHS,RHS>::result;
    
    
    template<typename VALUE>
    using opposite = mpl::sub<mpl::zero<VALUE>,VALUE>;

    template<typename VALUE>
    using increment = mpl::add<VALUE,mpl::one<VALUE>>;

    template<typename VALUE>
    using decrement = mpl::sub<VALUE,mpl::one<VALUE>>;

    template<typename BASE , typename EXPONENT>
    struct pow : public mpl::mul<BASE , pow<BASE,mpl::decrement<EXPONENT>>> {};

    template<typename BASE>
    struct pow<BASE,zero<BASE>> : public mpl::one<BASE> {};

    template<typename BASE>
    using square = mpl::mul<BASE,BASE>;
}
	/* OPERATORS_HPP */

