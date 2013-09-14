/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "list.hpp"
#include "control_flow.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"
#include "fixed_point.hpp"
#include "vector.hpp"
#include "sin.hpp"

#include <iostream>

using list = mpl::list<char,int,double>;

template<typename T , typename U>
struct comparer : public mpl::boolean<(sizeof(T) > sizeof(U))> {};

using sorted_list = mpl::sort<list , comparer>;


using zero = mpl::decimal<0>;
using pi = mpl::decimal<3141592 , -6>;
using pi_2 = mpl::div<pi,mpl::decimal<2>>;
using pi_4 = mpl::div<pi,mpl::decimal<4>>;

using one = mpl::decimal<1>;
using two = mpl::decimal<2>;
using one_two = decltype( one() / two() );

using iterations = mpl::uinteger<10>;

int main()
{
    std::cout << mpl::to_string<list>() << std::endl;
    std::cout << mpl::to_string<sorted_list>() << std::endl;
    std::cout << mpl::to_string<one_two>() << std::endl;
    std::cout << "sin(" << mpl::to_string<pi>() << ") = " << mpl::to_string<math::sin<pi,iterations>>() << std::endl;
    std::cout << "sin(" << mpl::to_string<pi_2>() << ") = " << mpl::to_string<math::sin<pi_2,iterations>>() << std::endl;
    std::cout << "sin(" << mpl::to_string<pi_4>() << ") = " << mpl::to_string<math::sin<pi_4,iterations>>() << std::endl;
}

