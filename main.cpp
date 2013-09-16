/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

//#define MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION 8

#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "list.hpp"
#include "control_flow.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"
#include "fixed_point.hpp"
#include "vector.hpp"
#include "trigonometry.hpp"
#include "matrix3x3.hpp"

#include <iostream>

using list = mpl::for_each<mpl::make_uinteger_forward_iterator<0>,mpl::make_uinteger_forward_iterator<10>,mpl::function>;

template<typename T , typename U>
struct comparer : public mpl::boolean<(sizeof(T) > sizeof(U))> {};

using sorted_list = mpl::sort<list , mpl::bigger_than>;


using zero = mpl::decimal<0>;
using pi_2 = mpl::div<math::pi,mpl::decimal<2>>;
using pi_4 = mpl::div<math::pi,mpl::decimal<4>>;

using one = mpl::decimal<1>;
using two = mpl::decimal<2>;
using one_two = decltype( one() / two() );

using iterations = mpl::uinteger<200>;



using unity = math::unity3x3<mpl::decimal<0>>;

int main()
{
    std::cout << mpl::to_string<list>() << std::endl;
    std::cout << mpl::to_string<sorted_list>() << std::endl;
    std::cout << mpl::to_string<one_two>() << std::endl;
    
    std::cout << "sin(" << mpl::to_string<math::pi>() << ") = " << mpl::to_string<math::sin<math::pi,iterations>>() << std::endl;
    std::cout << "sin(" << mpl::to_string<pi_2>() << ") = " << mpl::to_string<math::sin<pi_2,iterations>>() << std::endl;
    std::cout << "sin(" << mpl::to_string<pi_4>() << ") = " << mpl::to_string<math::sin<pi_4,iterations>>() << std::endl;
    
    std::cout << "cos(" << mpl::to_string<math::pi>() << ") = " << mpl::to_string<math::cos<math::pi,iterations>>() << std::endl;
    std::cout << "cos(" << mpl::to_string<pi_2>() << ") = " << mpl::to_string<math::cos<pi_2,iterations>>() << std::endl;
    std::cout << "cos(" << mpl::to_string<pi_4>() << ") = " << mpl::to_string<math::cos<pi_4,iterations>>() << std::endl;
    
    std::cout << "tan(" << mpl::to_string<math::pi>() << ") = " << mpl::to_string<math::tan<math::pi,iterations>>() << std::endl;
    std::cout << "tan(" << mpl::to_string<pi_2>() << ") = " << mpl::to_string<math::tan<pi_2,iterations>>() << std::endl;
    std::cout << "tan(" << mpl::to_string<pi_4>() << ") = " << mpl::to_string<math::tan<pi_4,iterations>>() << std::endl;
    
    std::cout << "cotan(" << mpl::to_string<math::pi>() << ") = " << mpl::to_string<math::cotan<math::pi,iterations>>() << std::endl;
    std::cout << "cotan(" << mpl::to_string<pi_2>() << ") = " << mpl::to_string<math::cotan<pi_2,iterations>>() << std::endl;
    std::cout << "cotan(" << mpl::to_string<pi_4>() << ") = " << mpl::to_string<math::cotan<pi_4,iterations>>() << std::endl;
    
    std::cout << mpl::to_string<decltype( unity() + unity()*mpl::decimal<123,-2>() )>() << std::endl;
}

