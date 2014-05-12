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

#include "variable_templates.hpp"
#include "sort.hpp"
#include "list.hpp"
#include "numeric_iterators.hpp"

#include <iostream>



using input = tml::for_each<tml::make_integer_forward_iterator<0>,tml::make_integer_forward_iterator<10>,tml::function>;
using output = tml::sort<input,tml::bigger_than>;


TURBO_INDEXED_VARIABLE( int , a );
TURBO_MUTABLE_VARIABLE( b );

int main()
{
    a<0>() = 10;
    a<3>() = 13;
    
    b<char>()  = 'a';
    b<float>() = 1.0f;
    
    std::cout << a<0>() << std::endl;
    std::cout << a<3>() << std::endl;
    std::cout << b<char>() << std::endl;
    std::cout << b<float>() << std::endl;
    
    std::cout << tml::to_string<input>() << std::endl;
    std::cout << tml::to_string<output>() << std::endl;
}

