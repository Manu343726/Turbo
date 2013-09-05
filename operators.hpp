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
    
    
    template<typename T>
    struct zero_t : public mpl::function<mpl::no_type> {};
    
    template<typename T>
    struct one_t : public mpl::function<T> {};
    
    template<typename T>
    using zero = typename mpl::zero_t<T>::result;
    
    template<typename T>
    using one = typename mpl::one_t<T>::result;
    
    
    
    template<typename T , T v>
    struct zero_t<mpl::value_t<T,v>> : public mpl::function<mpl::value_t<T,0>> {};
    
    template<typename T , T v>
    struct one_t<mpl::value_t<T,v>> : public mpl::function<mpl::value_t<T,1>> {};
    
    

    /* Logical functions */

    template<typename OP>
    using logical_not = mpl::function<mpl::boolean<!OP::value>>;

    template<typename LHS , typename RHS>
    struct logical_or  : public mpl::function<mpl::boolean<LHS::value || RHS::value>> {};
    
    template<typename LHS , typename RHS>
    struct logical_and  : public mpl::function<mpl::boolean<LHS::value && RHS::value>> {};

    template<typename LHS , typename RHS>
    struct logical_xor  : public mpl::function<mpl::boolean<LHS::value ^ RHS::value>> {};
    
    


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
    struct equal_t : public mpl::function<std::is_same<LHS,RHS>> {}; //Default

    template<typename T , T a , typename U , U b>
    struct equal_t<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::function<mpl::boolean<(a == b)>> {};
    
    template<typename LHS , typename RHS>
    using equal = typename mpl::equal_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using not_equal = logical_not<mpl::equal<LHS,RHS>>;

    template<typename LHS , typename RHS>
    struct less_than_t;

    template<typename T , T a , typename U , U b>
    struct less_than_t<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::function<mpl::boolean<(a < b)>> {};
    
    template<typename LHS , typename RHS>
    using less_than = typename mpl::less_than_t<LHS,RHS>::result;

    template<typename LHS , typename RHS>
    using bigger_than = mpl::less_than<RHS,LHS>;

    template<typename LHS , typename RHS>
    using less_or_equal = logical_not<bigger_than<LHS,RHS>>;

    template<typename LHS , typename RHS>
    using bigger_or_equal = logical_not<less_than<LHS,RHS>>;


    /* Arithmetic functions */
    
    template<typename LHS , typename RHS>
    struct add_t: public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value + RHS::value>> {}; 

    template<typename LHS , typename RHS>
    struct sub_t : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value - RHS::value>> {}; 

    template<typename LHS , typename RHS>
    struct mul_t : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value * RHS::value>> {}; 

    template<typename LHS , typename RHS>
    struct div_t : public mpl::function<mpl::value_t<typename LHS::value_type , LHS::value / RHS::value>> {}; 
    
    template<typename LHS , typename RHS>
    using add = typename mpl::add_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using sub = typename mpl::sub_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using mul = typename mpl::mul_t<LHS,RHS>::result;
    
    template<typename LHS , typename RHS>
    using div = typename mpl::div_t<LHS,RHS>::result;
    
    
    

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
#endif	/* OPERATORS_HPP */

