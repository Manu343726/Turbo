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
#include "for_loops.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"
#include "fixed_point.hpp"
#include "vector.hpp"
#include "trigonometry.hpp"
#include "matrix3x3.hpp"
#include "matrix4x4.hpp"
#include "prime_filter.hpp"
#include "color.hpp"
#include "sqrt.hpp"

#include <iostream>

template<typename T>
using filter = math::is_prime<T>;

using list = mpl::for_each<mpl::make_uinteger_forward_iterator<0>,mpl::make_uinteger_forward_iterator<2>,mpl::function,filter>;

template<typename T , typename U>
struct comparer : public mpl::boolean<(sizeof(T) > sizeof(U))> {};

using sorted_list = mpl::sort<list , mpl::bigger_than>;

using pi_2 = mpl::div<math::pi,mpl::decimal<2>>;
using pi_4 = mpl::div<math::pi,mpl::decimal<4>>;

using iterations = mpl::uinteger<100>;



using unity = math::unity3x3<mpl::decimal<0>>;

using vector_2d = math::vec2<mpl::decimal<10> , mpl::decimal<0>>;
using vector_3d = math::vec3<mpl::decimal<10> , mpl::decimal<10> , mpl::decimal<10>>;

using rotation_2d = math::rotate<pi_2,math::rotation_2d,iterations>;
using rotation_3d = math::rotate<pi_2,math::x_axis,iterations>;

using color = gfx::color<gfx::byte<255> , gfx::byte<0> , gfx::byte<0>>;

int main()
{  
    std::cout << mpl::to_string<decltype(math::translate<math::vec2<mpl::decimal<1>,mpl::decimal<1>>>() * rotation_2d() * vector_2d())>() << std::endl;
    std::cout << mpl::to_string<decltype(rotation_3d() * vector_3d())>() << std::endl;
    
    std::cout << mpl::to_string<color>() << std::endl;
    
    std::cout << mpl::to_string<math::sqrt<mpl::decimal<25>>>() << std::endl;
}

