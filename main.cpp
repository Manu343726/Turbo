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

#include <iostream>


using input = mpl::list<char,int,float,bool>; //The input is a list of basic types

template<typename T , typename U>
using comparer = mpl::boolean<(sizeof(T) > sizeof(U))>; //In this example, we use the size of the type as ordering criteria (Decreasing order, biggest element to the left and lessr to the right )

using output = mpl::sort<input,comparer>; //Sorts the list using that comparer

template<typename T>
struct kernel
{
    using result = mpl::size_t<sizeof(T)>;
};

/* 
 * mpl::for_each returns a list wich contains the set of results of aplying an specified kernel to each element of a specified interval.
 * In this case, retuns a list with the sizes of the types in the interval [begin + 2 , end)
 */
using sizes = mpl::for_each<decltype( mpl::begin<output>() + mpl::size_t<2>() ) , mpl::end<output> , kernel>;


using begin = mpl::make_uinteger_forward_iterator<0>;
using end   = mpl::make_uinteger_forward_iterator<10>;

template<typename CURRENT , typename PREVIOUS_RESULT>
struct fibonacci_kernel
{
    using fib_n_1 = typename PREVIOUS_RESULT::first;
    using fib_n_2 = typename PREVIOUS_RESULT::second;
};



using pi = mpl::decimal<3141592,-6>;
using pi_2 = decltype( pi() * mpl::integer<2>() );

int main()
{
    std::cout << mpl::to_string<pi>() << std::endl;
    std::cout << mpl::to_string<pi_2>() << std::endl;
    std::cout << mpl::to_string<input>() << std::endl;  //This prints [char,int,float,bool]
    std::cout << mpl::to_string<output>() << std::endl; //This prints [float,int,char,bool]
    std::cout << mpl::to_string<sizes>() << std::endl;  //This prints [1,1]
}
