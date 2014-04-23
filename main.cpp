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

#include "impl/demangle.hpp"

#include <type_traits>
#include <typeinfo>
#include <iostream>

using namespace tml::placeholders;

template<typename A, typename B, typename C>
struct f
{
    using result = decltype(nullptr);
};


//tml::let ussage example:
struct X {}; //Variable
struct Y {}; //Variable
struct Z {}; //Variable

using let = tml::multi_let<X,Y,Z,
                           float,int,double,
                           f<X,Y,Z>
                          >;

using result = tml::eval<let>;

int main()
{
    std::cout << tml::impl::demangle( typeid( let ).name() ) << std::endl;
}

