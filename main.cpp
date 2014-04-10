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

#include "functional.hpp"
#include "placeholders.hpp"
#include "let_expressions.hpp"

#include <type_traits>
#include <typeinfo>
#include <iostream>

using namespace tml::placeholders;

template<typename... ARGS>
struct f
{
    using result = decltype(nullptr);
};

template<typename ARG , typename... ARGS> using first_of = ARG;

using char_alias = tml::eval<char>; //the result must be a char alias
using first = tml::eval<f<_1,_2,_3,_4,_5> , bool,int,double,char,float>;


//tml::let ussage example:
struct X {}; //Variable

using let = tml::let<X,int,f<X,X,X>>;

using result = tml::eval<let>;

int main()
{
    std::cout << typeid( let ).name();
}

