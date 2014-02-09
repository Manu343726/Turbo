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

#include "core.hpp"

TURBO_DEFINE_FUNCTION( first_of , (typename T , typename U) , (T , U) );

//Default "overload":
template<typename T , typename U>
struct first_of_t
{
    using result = T;
};

//First argumment is bool "overload":
template<typename U>
struct first_of_t<bool,U> : public tml::function<U> {}; //haha, just confuse people when using booleans!

int main()
{
    first_of<char,bool> a;
    first_of<bool,char> b;
    
    static_assert( tml::equal<decltype(a) , char>::value , "boom!" );
    static_assert( tml::equal<decltype(a) , bool>::value , "haha, you lose!" );
}

