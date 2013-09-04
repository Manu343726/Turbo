/* 
 * File:   functions.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 20:39
 */

#ifndef OPERATORS_HPP
#define	OPERATORS_HPP

#include "basic_types.hpp"

#include <type_traits>

namespace mpl
{
    template<typename RESULT>
    struct function : public RESULT
    {
        using result = RESULT;
    };
    

    /* Logical functions */

    template<typename OP>
    using logical_not = mpl::function<mpl::boolean<!OP::value>>;

    template<typename... OPERANDS>
    struct logical_or;

    template<typename LHS , typename RHS>
    struct logical_or<LHS,RHS>  : public mpl::function<mpl::boolean<LHS::value || RHS::value>> {};

    template<typename FIRST , typename... REST>
    struct logical_or<FIRST,REST...> : public logical_or<FIRST,logical_or<REST...>> {};

    template<typename... OPERANDS>
    struct logical_and;

    template<typename LHS , typename RHS>
    struct logical_and<LHS,RHS>  : public mpl::function<mpl::boolean<LHS::value && RHS::value>> {};

    template<typename FIRST , typename... REST>
    struct logical_and<FIRST,REST...> : public logical_and<FIRST,logical_and<REST...>> {};

    template<typename... OPERANDS>
    struct logical_xor;

    template<typename LHS , typename RHS>
    struct logical_xor<LHS,RHS>  : public mpl::function<mpl::boolean<LHS::value ^ RHS::value>> {};

    template<typename FIRST , typename... REST>
    struct logical_xor<FIRST,REST...> : public logical_xor<FIRST,logical_xor<REST...>> {};


    /* Bitwise functions */

    template<typename OP , typename SHIFT>
    using bitwise_leftshift = mpl::function<mpl::value_t<OP , (OP::value << SHIFT::value)>>;

    template<typename OP , typename SHIFT>
    using bitwise_rightshift = mpl::function<mpl::value_t<OP , (OP::value >> SHIFT::value)>>;

    template<typename OP>
    using bitwise_not = mpl::function<mpl::value_t<OP , compl OP::value>>; //http://en.cppreference.com/w/cpp/language/function_alternative

    template<typename LHS , typename RHS>
    using bitwise_or = mpl::function<mpl::value_t<LHS , LHS::value | RHS::value>>;

    template<typename LHS , typename RHS>
    using bitwise_and = mpl::function<mpl::value_t<LHS , LHS::value & RHS::value>>;

    template<typename LHS , typename RHS>
    using bitwise_xor = mpl::function<mpl::value_t<LHS , LHS::value ^ RHS::value>>;


    /* Comparison functions */

    template<typename LHS , typename RHS>
    struct equal : public mpl::function<std::is_same<LHS,RHS>> {}; //Default

    template<typename T , T a , typename U , U b>
    struct equal<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::boolean<(a == b)> {};

    template<typename LHS , typename RHS>
    using not_equal = logical_not<equal<LHS,RHS>>;

    template<typename LHS , typename RHS>
    struct less_than;

    template<typename T , T a , typename U , U b>
    struct less_than<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::boolean<(a < b)> {};

    template<typename LHS , typename RHS>
    using bigger_than = less_than<RHS,LHS>;

    template<typename LHS , typename RHS>
    using less_or_equal = logical_not<bigger_than<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using bigger_or_equal = logical_not<less_than<LHS,RHS>>;


    /* Arithmetic functions */

    template<typename... OPERANDS>
    struct add;
    
    template<typename LHS , typename RHS>
    struct add<LHS,RHS> : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value + RHS::value>> {}; 

    template<typename HEAD , typename... TAIL>
    struct add<HEAD,TAIL...> : public mpl::add<HEAD,add<TAIL...>> {};

    template<typename LHS , typename RHS>
    struct sub : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value - RHS::value>> {}; 

    template<typename LHS , typename RHS>
    struct mul : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value * RHS::value>> {}; 

    template<typename LHS , typename RHS>
    struct div : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value / RHS::value>> {}; 

    template<typename VALUE>
    struct increment : public add<VALUE,mpl::one<VALUE>> {};

    template<typename VALUE>
    struct decrement : public sub<VALUE,mpl::one<VALUE>> {};

    template<typename BASE , typename EXPONENT>
    struct pow : public mul<BASE , pow<BASE,decrement<EXPONENT>>> {};

    template<typename BASE>
    struct pow<BASE,zero<BASE>> : public mpl::one<BASE> {};

    template<typename BASE>
    struct square : public mul<BASE,BASE> {};
}
#endif	/* OPERATORS_HPP */

