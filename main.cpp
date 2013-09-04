/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

#include "newfile.hpp"
#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "list.hpp"
#include "control_flow.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"

#include <iostream>


using input = mpl::list<char,int,float>;

template<typename T , typename U>
using comparer = mpl::boolean<(sizeof(T) < sizeof(U))>;

using sorted = mpl::sort<input,comparer>;

int main()
{
    std::cout << mpl::to_string<input>() << std::endl;
    std::cout << mpl::to_string<sorted>() << std::endl;
}
