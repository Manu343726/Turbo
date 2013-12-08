/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

//#define MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION 8

#include "analogliterals.hpp"
#include "to_string.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"

#include <iostream>

using begin = mpl::make_integer_forward_iterator<-10>;
using end   = mpl::make_integer_forward_iterator<10>;

using numbers = mpl::for_each<begin,end,mpl::function>;
using sorted  = mpl::sort<numbers,mpl::bigger_than>;

int main()
{
    using namespace mpl::utils::analog_literals::shapes;
    using namespace mpl::utils::analog_literals::symbols;
    
    auto rectangle = (o---------------------o
                      |                     !
                      !                     !
                      !                     !
                      !                     !
                      !                     !
                      !                     !
                      !                     !
                      o---------------------o) * decltype( mpl::uinteger<4>() * mpl::uinteger<10>() )();
    
    std::cout << rectangle.width << "," << rectangle.height << std::endl;
    
    std::cout << mpl::to_string<numbers>() << std::endl;
    std::cout << mpl::to_string<sorted>() << std::endl;
}
