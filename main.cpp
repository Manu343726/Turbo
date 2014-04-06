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

#include <type_traits>

struct _1; //placeholder

using char_alias = tml::eval<char>; //the result must be a char alias

template<typename T> using unity = tml::function<T>; //unity( T ) -> T

using function_call = tml::eval<unity<_1>,char>; //the result must be a char alias

static_assert( std::is_same<char,char_alias>::value && 
               std::is_same<char,function_call>::value , 
               "Something failed during evaluation" );

int main()
{
    
}

