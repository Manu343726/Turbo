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

struct my_type{};

TURBO_DEFINE_FUNCTION( is_my_type , (typename T) , (T) , (tml::true_type) );
TURBO_DEFINE_FUNCTION( something  , (typename T) , (T) );

TURBO_ASSERT( (tml::equal<tml::size_of<char>,tml::size_t<1>>) , "non standard-compilant compiler?" );
TURBO_ASSERT( (is_my_type<my_type>) );

int main(){}

